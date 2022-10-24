
#include "VulkanRenderCommandBuffer.h"
#ifdef HZR_INCLUDE_VULKAN
#include "VulkanContext.h"
#include "Backend/Core/Renderer.h"


#include "VulkanFramebuffer.h"
#include "Pipeline/VulkanVertexBuffer.h"
#include "Pipeline/VulkanIndexBuffer.h"
#include "Pipeline/VulkanUniformBuffer.h"
#include "Pipeline/VulkanPipeline.h"
#include "Pipeline/VulkanShader.h"

#include "VkUtils.h"
#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::Vulkan
{
	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(uint32_t size, const std::string& name) : m_DebugName(std::move(name))
	{
		auto& device = VulkanContext::GetLogicalDevice();
		uint32_t framesInFlight = VulkanContext::GetImagesInFlight();

		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = device->GetPhysicalDevice().As<VulkanPhysicalDevice>()->GetQueueFamilyIndices().Graphics;
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		VK_CHECK_RESULT(vkCreateCommandPool(device->GetVulkanDevice(), &cmdPoolInfo, nullptr, &m_CommandPool), "Failed to create Command Pool");

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = size == 0 ? framesInFlight : size;

		m_CommandBuffers.resize(allocInfo.commandBufferCount);

		VK_CHECK_RESULT(vkAllocateCommandBuffers(device->GetVulkanDevice(), &allocInfo, m_CommandBuffers.data()), "Failed to allocate Command Buffers");

		for (size_t i = 0; i < m_CommandBuffers.size(); i++)
			VkUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(), VK_OBJECT_TYPE_COMMAND_BUFFER, fmt::format("{} (Frame In Flight: {})", m_DebugName), m_CommandBuffers[i]);

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		m_WaitFences.resize(m_CommandBuffers.size());
		for (size_t i = 0; i < m_WaitFences.size(); i++)
		{
			VK_CHECK_RESULT(vkCreateFence(device->GetVulkanDevice(), &fenceInfo, nullptr, &m_WaitFences[i]), "Failed to create VkFence");
			VkUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(), VK_OBJECT_TYPE_FENCE, fmt::format("{} (Frame In Flight: {}) VkFence", m_DebugName), m_WaitFences[i]);
		}

		VkQueryPoolCreateInfo queryPoolCreateInfo = {};
		queryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;

		const uint32_t maxQueries = 16;
		m_TimestampQueryCount = 2 + 2 * maxQueries;
		queryPoolCreateInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
		queryPoolCreateInfo.queryCount = m_TimestampQueryCount;

		m_TimestampQueryPools.resize(m_TimestampQueryCount);
		for (auto& pool : m_TimestampQueryPools)
			VK_CHECK_RESULT(vkCreateQueryPool(device->GetVulkanDevice(), &queryPoolCreateInfo, nullptr, &pool), "Failed to create QueryPool");

		m_TimestampQueryResults.resize(framesInFlight);
		for (auto& result : m_TimestampQueryResults)
			result.reserve(m_TimestampQueryCount);

		m_GPUExecutionTimes.resize(framesInFlight);
		for (auto& time : m_GPUExecutionTimes)
			time.resize(m_TimestampQueryCount / 2);

		m_PipelineQueryCount = 7;

		queryPoolCreateInfo.queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS;
		queryPoolCreateInfo.queryCount = m_PipelineQueryCount;
		queryPoolCreateInfo.pipelineStatistics =
			VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;

		m_PipelineQueryPools.resize(framesInFlight);
		for (auto& queryPool : m_PipelineQueryPools)
			VK_CHECK_RESULT(vkCreateQueryPool(device->GetVulkanDevice(), &queryPoolCreateInfo, nullptr, &queryPool), "Failed to create QueryPool");

		m_PipelineStatisticQueryResults.resize(framesInFlight);
	}
	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(const std::string& name, bool swapchain) : m_DebugName(std::move(name)), m_OwnedBySwapchain(swapchain)
	{
		auto& device = VulkanContext::GetLogicalDevice();
		uint32_t framesInFlight = VulkanContext::GetImagesInFlight();

		VkQueryPoolCreateInfo queryPoolCreateInfo = {};
		queryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;

		const uint32_t maxQueries = 16;
		m_TimestampQueryCount = 2 + 2 * maxQueries;
		queryPoolCreateInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
		queryPoolCreateInfo.queryCount = m_TimestampQueryCount;

		m_TimestampQueryPools.resize(m_TimestampQueryCount);
		for (auto& pool : m_TimestampQueryPools)
			VK_CHECK_RESULT(vkCreateQueryPool(device->GetVulkanDevice(), &queryPoolCreateInfo, nullptr, &pool), "Failed to create QueryPool");

		m_TimestampQueryResults.resize(framesInFlight);
		for (auto& result : m_TimestampQueryResults)
			result.reserve(m_TimestampQueryCount);

		m_GPUExecutionTimes.resize(framesInFlight);
		for (auto& time : m_GPUExecutionTimes)
			time.resize(m_TimestampQueryCount / 2);

		m_PipelineQueryCount = 7;

		queryPoolCreateInfo.queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS;
		queryPoolCreateInfo.queryCount = m_PipelineQueryCount;
		queryPoolCreateInfo.pipelineStatistics =
			VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;

		m_PipelineQueryPools.resize(framesInFlight);
		for (auto& queryPool : m_PipelineQueryPools)
			VK_CHECK_RESULT(vkCreateQueryPool(device->GetVulkanDevice(), &queryPoolCreateInfo, nullptr, &queryPool), "Failed to create QueryPool");

		m_PipelineStatisticQueryResults.resize(framesInFlight);
	}
	VulkanRenderCommandBuffer::~VulkanRenderCommandBuffer()
	{
		if (m_OwnedBySwapchain) return;

		VkCommandPool commandPool = m_CommandPool;
		Renderer::SubmitResourceFree([commandPool]() mutable {
			auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
			vkDestroyCommandPool(device, commandPool, nullptr);
			});
	}
	void VulkanRenderCommandBuffer::Begin()
	{
		m_TimestampNextAvailQuery = 2;
		Ref<VulkanSwapchain> swapchain = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>();

		uint32_t frameIndex = swapchain->GetCurrentBufferIndex();

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VkCommandBuffer buffer;
		if (m_OwnedBySwapchain)
			buffer = swapchain->GetCurrentDrawCommandBuffer();
		else
			buffer = m_CommandBuffers[frameIndex];

		m_ActiveCommandBuffer = buffer;

		VK_CHECK_RESULT(vkBeginCommandBuffer(buffer, &beginInfo), "Failed to begin Command Buffer");

		//vkCmdResetQueryPool(buffer, m_TimestampQueryPools[frameIndex], 0, m_TimestampQueryCount);
		//vkCmdWriteTimestamp(buffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, m_TimestampQueryPools[frameIndex], 0);

		//vkCmdResetQueryPool(buffer, m_PipelineQueryPools[frameIndex], 0, m_PipelineQueryCount);
		//vkCmdBeginQuery(buffer, m_PipelineQueryPools[frameIndex], 0, 0);
	}
	void VulkanRenderCommandBuffer::End()
	{
		uint32_t frameIndex = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>()->GetCurrentBufferIndex();

		VkCommandBuffer commandBuffer = m_ActiveCommandBuffer;
		//vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, m_TimestampQueryPools[frameIndex], 1);
		//vkCmdEndQuery(commandBuffer, m_PipelineQueryPools[frameIndex], 0);
		VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer), "Failed to end command buffer");

		m_ActiveCommandBuffer = nullptr;
	}
	void VulkanRenderCommandBuffer::Submit()
	{
		if (m_OwnedBySwapchain) return;
	}
	void VulkanRenderCommandBuffer::BeginRenderPass(Ref<RenderPass> renderPass, bool explicitClear)
	{
		Ref<VulkanRenderCommandBuffer> instance = this;
		Ref<RenderPass> pass = renderPass;
		Renderer::Submit([instance, pass, explicitClear]() mutable {
			instance->BeginRenderPass_RT(pass, explicitClear);
			});
	}
	void VulkanRenderCommandBuffer::BeginRenderPass_RT(Ref<RenderPass> renderPass, bool explicitClear)
	{
		auto& swapchain = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>();
		auto& fb = renderPass->GetSpecs().TargetFrameBuffer.As<VulkanFrameBuffer>();

		uint32_t w = fb->GetWidth();
		uint32_t h = fb->GetHeight();

		VkViewport viewport = {};
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRenderPassBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderArea.offset.x = 0;
		beginInfo.renderArea.offset.y = 0;
		beginInfo.renderArea.extent.width = w;
		beginInfo.renderArea.extent.height = h;
		beginInfo.renderPass = fb->GetRenderPass();

		if (fb->GetSpecification().SwapChainTarget)
		{
			w = swapchain->GetWidth();
			h = swapchain->GetHeight();
			beginInfo.framebuffer = swapchain->GetCurrentFramebuffer();

			viewport.x = 0.0f;
			viewport.y = (float)h;
			viewport.width = (float)w;
			viewport.height = -(float)h;
		}
		else
		{
			beginInfo.framebuffer = fb->GetVulkanFramebuffer();

			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)w;
			viewport.height = (float)h;
		}

		const auto& clearValues = fb->GetClearValues();
		beginInfo.clearValueCount = (uint32_t)clearValues.size();
		beginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_ActiveCommandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

		if (explicitClear)
		{
			const uint32_t colorAttachmentCount = fb->GetColorAttachmentCount();
			const uint32_t totalAttachmentCount = colorAttachmentCount + fb->HasDepthAttachment();
		}

		vkCmdSetViewport(m_ActiveCommandBuffer, 0, 1, &viewport);

		VkRect2D scissors = {};
		scissors.offset = { 0, 0 };
		scissors.extent = { w, h };
		vkCmdSetScissor(m_ActiveCommandBuffer, 0, 1, &scissors);
	}
	void VulkanRenderCommandBuffer::EndRenderPass()
	{
		Ref<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance]() {
			vkCmdEndRenderPass(instance->m_ActiveCommandBuffer);
			});
	}
	void VulkanRenderCommandBuffer::BindVertexBuffer(Ref<VertexBuffer> vertexBuffer, uint32_t binding)
	{
		Ref<VulkanVertexBuffer> buffer = vertexBuffer.As<VulkanVertexBuffer>();
		Ref<VulkanRenderCommandBuffer> instance = this;

		Renderer::Submit([instance, buffer, binding]() mutable {
			VkBuffer vkBuffer = buffer->GetVulkanBuffer();
			VkDeviceSize offsets = { 0 };
			vkCmdBindVertexBuffers(instance->m_ActiveCommandBuffer, 0, 1, &vkBuffer, &offsets);
			});
	}
	void VulkanRenderCommandBuffer::BindUniformBuffer(Ref<UniformBuffer> uniformBuffer)
	{

	}
	void VulkanRenderCommandBuffer::BindPipeline(Ref<Pipeline> pipeline)
	{
		Ref<VulkanPipeline> vkPipeline = pipeline.As<VulkanPipeline>();
		auto& shader = pipeline->GetShader().As<VulkanShader>();

		auto& offsets = shader->GetDynamicOffsets();
		Ref<VulkanRenderCommandBuffer> instance = this;

		Renderer::Submit([instance, pipeline = vkPipeline, offsets, shader]() mutable {

			auto bindPoint = pipeline->GetBindingPoint();
			auto layout = pipeline->GetPipelineLayout();
			auto vkPipeline = pipeline->GetVulkanPipeline();
			
			auto& descriptorSets = shader->GetDescriptorSets();

			vkCmdBindDescriptorSets(instance->m_ActiveCommandBuffer, bindPoint, layout, 0, descriptorSets.size(), descriptorSets.data(), offsets.size(), offsets.data());
			vkCmdBindPipeline(instance->m_ActiveCommandBuffer, bindPoint, vkPipeline);
			});
	}
	void VulkanRenderCommandBuffer::Draw(uint32_t count, Ref<IndexBuffer> indexBuffer)
	{
		DrawInstanced(count, 1, indexBuffer);
	}
	void VulkanRenderCommandBuffer::DrawInstanced(uint32_t count, uint32_t instanceCount, Ref<IndexBuffer> indexBuffer)
	{
		Ref<VulkanIndexBuffer> buffer = indexBuffer.As<VulkanIndexBuffer>();
		Ref<VulkanRenderCommandBuffer> instance = this;

		Renderer::Submit([instance, buffer, count, instanceCount]() mutable {

			if (buffer)
			{
				VkDeviceSize offsets = { 0 };
				vkCmdBindIndexBuffer(instance->m_ActiveCommandBuffer, buffer->GetVulkanBuffer(), offsets, VK_INDEX_TYPE_UINT32);
				vkCmdDrawIndexed(instance->m_ActiveCommandBuffer, count, instanceCount, 0, 0, 0);
				return;
			}
			vkCmdDraw(instance->m_ActiveCommandBuffer, count, instanceCount, 0, 0);
			});
	}
	void VulkanRenderCommandBuffer::DispatchCompute(const LocalGroupSize& localGroupSize)
	{
		Ref<VulkanRenderCommandBuffer> instance = this;

		Renderer::Submit([instance, size = localGroupSize]() mutable {
				vkCmdDispatch(instance->m_ActiveCommandBuffer, size.x, size.y, size.z);
			});
	}
	void VulkanRenderCommandBuffer::InsertMemoryBarrier(MemoryBarrierFlags flags)
	{

	}
	void VulkanRenderCommandBuffer::SetViewport(float x, float y, float width, float height)
	{

	}
	void VulkanRenderCommandBuffer::SetLineSize(float size)
	{
		Ref<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance, size]() mutable {
			vkCmdSetLineWidth(instance->m_ActiveCommandBuffer, size);
			});
	}
}
#endif