
#include "VulkanRenderCommandBuffer.h"
#ifdef HZR_INCLUDE_VULKAN
#include "VulkanContext.h"
#include "Backend/Core/Renderer.h"


#include "VulkanFramebuffer.h"
#include "Pipeline/VulkanGPUBuffer.h"
#include "Pipeline/VulkanPipeline.h"
#include "Pipeline/VulkanShader.h"
#include "Textures/VulkanCubemapTexture.h"
#include "RTCore/VulkanShaderBindingTable.h"
#include "RTCore/VulkanTopLevelAS.h"
#include "RTCore/VulkanBottomLevelAS.h"

#include "VkUtils.h"
#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::Vulkan
{
	static PFN_vkCmdTraceRaysKHR fpCmdTraceRaysKHR;

	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(const std::string& name, DeviceQueue queue, uint32_t count) : m_DebugName(std::move(name))
	{
		HZR_PROFILE_FUNCTION();

		auto device = VulkanContext::GetLogicalDevice();

		uint32_t framesInFlight = VulkanContext::GetImagesInFlight();

		auto queueIndices = device->GetPhysicalDevice().As<VulkanPhysicalDevice>()->GetQueueFamilyIndices();

		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		switch (queue)
		{
		case HazardRenderer::DeviceQueue::GraphicsBit:
			m_SubmitQueue = device->GetGraphicsQueue();
			cmdPoolInfo.queueFamilyIndex = queueIndices.Graphics;
			break;
		case HazardRenderer::DeviceQueue::ComputeBit:
			m_SubmitQueue = device->GetComputeQueue();
			cmdPoolInfo.queueFamilyIndex = queueIndices.Compute;
			break;
		case HazardRenderer::DeviceQueue::TransferBit:
			m_SubmitQueue = device->GetGraphicsQueue();
			cmdPoolInfo.queueFamilyIndex = queueIndices.Graphics;
			break;
		}


		VK_CHECK_RESULT(vkCreateCommandPool(device->GetVulkanDevice(), &cmdPoolInfo, nullptr, &m_CommandPool), "Failed to create Command Pool");

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = count == 0 ? framesInFlight : count;

		m_CommandBuffers.resize(allocInfo.commandBufferCount);

		VK_CHECK_RESULT(vkAllocateCommandBuffers(device->GetVulkanDevice(), &allocInfo, m_CommandBuffers.data()), "Failed to allocate Command Buffers");

		for (size_t i = 0; i < m_CommandBuffers.size(); i++)
			VkUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(), VK_OBJECT_TYPE_COMMAND_BUFFER, fmt::format("CommandBuffer {}", m_DebugName), m_CommandBuffers[i]);

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		m_WaitFences.resize(m_CommandBuffers.size());
		for (size_t i = 0; i < m_WaitFences.size(); i++)
		{
			VK_CHECK_RESULT(vkCreateFence(device->GetVulkanDevice(), &fenceInfo, nullptr, &m_WaitFences[i]), "Failed to create VkFence");
			VkUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(), VK_OBJECT_TYPE_FENCE, fmt::format("{} (Frame In Flight: {}) VkFence", m_DebugName, i), m_WaitFences[i]);
		}

		//----- Query pools -----

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
			result.resize(m_TimestampQueryCount);

		m_GPUExecutionTimes.resize(framesInFlight);
		for (auto& time : m_GPUExecutionTimes)
			time.resize(m_TimestampQueryCount / 2);

		m_PipelineQueryCount = 7;

		queryPoolCreateInfo.queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS;
		queryPoolCreateInfo.queryCount = m_PipelineQueryCount;
		if (queue == DeviceQueue::GraphicsBit)
		{
			queryPoolCreateInfo.pipelineStatistics =
				VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
				VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
				VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
				VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
				VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT |
				VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT;

		}
		else if (queue == DeviceQueue::ComputeBit)
		{
			queryPoolCreateInfo.pipelineStatistics =
				VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;
		}

		m_PipelineQueryPools.resize(framesInFlight);
		for (auto& queryPool : m_PipelineQueryPools)
			VK_CHECK_RESULT(vkCreateQueryPool(device->GetVulkanDevice(), &queryPoolCreateInfo, nullptr, &queryPool), "Failed to create QueryPool");

		m_PipelineStatisticQueryResults.resize(framesInFlight);
	}
	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(const std::string& name, bool swapchain) : m_DebugName(std::move(name)), m_OwnedBySwapchain(swapchain)
	{
		HZR_PROFILE_FUNCTION();
		auto device = VulkanContext::GetLogicalDevice();

		if (device->GetPhysicalDevice()->SupportsRaytracing())
			GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), CmdTraceRaysKHR);

		uint32_t framesInFlight = VulkanContext::GetImagesInFlight();

		//----- Query pools -----

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
			result.resize(m_TimestampQueryCount);

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

		m_ActiveCommandBuffer = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>()->GetCurrentDrawCommandBuffer();

		m_SubmitQueue = device->GetGraphicsQueue();
	}
	VulkanRenderCommandBuffer::~VulkanRenderCommandBuffer()
	{
		HZR_PROFILE_FUNCTION();
		auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		for (auto queryPool : m_TimestampQueryPools)
			vkDestroyQueryPool(device, queryPool, nullptr);

		for (auto queryPool : m_PipelineQueryPools)
			vkDestroyQueryPool(device, queryPool, nullptr);

		for (auto& result : m_PipelineStatisticQueryResults)
			result.clear();

		if (m_OwnedBySwapchain) return;
		if (!m_CommandPool) return;

		VkCommandPool commandPool = m_CommandPool;
		vkDestroyCommandPool(device, commandPool, nullptr);
	}
	void VulkanRenderCommandBuffer::Begin()
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanRenderCommandBuffer> instance = this;

		if (m_OwnedBySwapchain)
		{
			instance->m_State = State::Record;
			Ref<VulkanSwapchain> swapchain = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>();
			uint32_t frameIndex = swapchain->GetCurrentBufferIndex();
			VkCommandBuffer buffer = swapchain->GetCurrentDrawCommandBuffer();

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			VK_CHECK_RESULT(vkBeginCommandBuffer(buffer, &beginInfo), "Failed to begin Command Buffer");
			instance->m_ActiveCommandBuffer = buffer;

			instance->BeginPerformanceQuery_RT();

			return;
		}

		Renderer::Submit([instance]() mutable {
			instance->m_TimestampNextAvailQuery = 2;
			instance->m_State = State::Record;

			VkCommandBuffer buffer = instance->m_CommandBuffers[instance->m_FrameIndex];

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			instance->m_ActiveCommandBuffer = buffer;

			VK_CHECK_RESULT(vkBeginCommandBuffer(buffer, &beginInfo), "Failed to begin Command Buffer");

			instance->BeginPerformanceQuery_RT();
			});
	}
	void VulkanRenderCommandBuffer::End()
	{
		HZR_PROFILE_FUNCTION();

		Ref<VulkanRenderCommandBuffer> instance = this;

		if (m_OwnedBySwapchain)
		{
			instance->EndPerformanceQuery_RT();
			vkEndCommandBuffer(m_ActiveCommandBuffer);
			m_State = State::Finished;
			return;
		}

		Renderer::Submit([instance]() mutable {
			HZR_ASSERT(instance->m_State == State::Record, "Command buffer not in recording state");
			instance->EndPerformanceQuery_RT();
			VK_CHECK_RESULT(vkEndCommandBuffer(instance->m_ActiveCommandBuffer), "Failed to end command buffer");
			instance->m_ActiveCommandBuffer = nullptr;
			instance->m_State = State::Finished;
			});
	}
	void VulkanRenderCommandBuffer::Submit()
	{
		HZR_PROFILE_FUNCTION();
		if (m_OwnedBySwapchain) return;

		Ref<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance]() mutable {
			HZR_TIMED_FUNCTION();
			HZR_ASSERT(instance->m_State == State::Finished, "Command buffer not in recording state");
			instance->m_State = State::Submit;
			auto device = VulkanContext::GetInstance()->GetLogicalDevice();

			if (!s_ComputeFence)
			{
				VkFenceCreateInfo fenceCreateInfo = {};
				fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

				VK_CHECK_RESULT(vkCreateFence(device->GetVulkanDevice(), &fenceCreateInfo, nullptr, &s_ComputeFence), "");
			}

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = instance->m_CommandBuffers.size();
			submitInfo.pCommandBuffers = instance->m_CommandBuffers.data();

			vkWaitForFences(device->GetVulkanDevice(), 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
			vkResetFences(device->GetVulkanDevice(), 1, &s_ComputeFence);

			VK_CHECK_RESULT(vkQueueSubmit(instance->m_SubmitQueue, 1, &submitInfo, s_ComputeFence), "");
			VK_CHECK_RESULT(vkWaitForFences(device->GetVulkanDevice(), 1, &s_ComputeFence, VK_TRUE, UINT64_MAX), "");

			instance->GetQueryPoolResults_RT();

			});
	}
	void VulkanRenderCommandBuffer::BeginRenderPass(Ref<RenderPass> renderPass, bool explicitClear)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanRenderCommandBuffer> instance = this;
		Ref<RenderPass> pass = renderPass;

		Renderer::Submit([instance, pass, explicitClear]() mutable {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::BeginRenderPass");
			instance->BeginRenderPass_RT(pass, explicitClear);
			});
	}
	void VulkanRenderCommandBuffer::BeginRenderPass_RT(Ref<RenderPass> renderPass, bool explicitClear)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(m_State == State::Record, "Command buffer not in recording state");
		auto fb = renderPass->GetSpecs().TargetFrameBuffer.As<VulkanFrameBuffer>();

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
			auto swapchain = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>();
			w = swapchain->GetWidth();
			h = swapchain->GetHeight();
			beginInfo.framebuffer = swapchain->GetCurrentFramebuffer();

			HZR_ASSERT(w <= 8192 && h <= 8192, "Too big image");

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
		HZR_PROFILE_FUNCTION();
		Ref<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance]() {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::EndRenderPass");
		HZR_ASSERT(instance->m_State == State::Record, "Command buffer not in recording state");
		vkCmdEndRenderPass(instance->m_ActiveCommandBuffer);
			});
	}
	void VulkanRenderCommandBuffer::SetVertexBuffer(Ref<GPUBuffer> vertexBuffer, uint32_t binding)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(vertexBuffer->GetUsageFlags() & BUFFER_USAGE_VERTEX_BUFFER_BIT, "Invalid buffer flags");

		Ref<VulkanGPUBuffer> buffer = vertexBuffer.As<VulkanGPUBuffer>();
		Ref<VulkanRenderCommandBuffer> instance = this;

		Renderer::Submit([instance, buffer, binding]() mutable {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::BindVertexBuffer");
		HZR_ASSERT(instance->m_State == State::Record, "Command buffer not in recording state");
		VkBuffer vkBuffer = buffer->GetVulkanBuffer();
		VkDeviceSize offsets = { 0 };

		vkCmdBindVertexBuffers(instance->m_ActiveCommandBuffer, binding, 1, &vkBuffer, &offsets);
			});
	}
	void VulkanRenderCommandBuffer::SetDescriptorSet(Ref<DescriptorSet> descriptorSet, uint32_t set)
	{
		Ref<VulkanRenderCommandBuffer> instance = this;
		Ref<VulkanDescriptorSet> vkSet = descriptorSet.As<VulkanDescriptorSet>();
		VkPipelineBindPoint bindingPoint = m_CurrentPipeline->GetBindingPoint();

		Renderer::Submit([instance, pipeline = m_CurrentPipeline, bindingPoint, vkSet, setIndex = set]() mutable {
			const VkDescriptorSet set = vkSet->GetVulkanDescriptorSet();
		vkCmdBindDescriptorSets(instance->m_ActiveCommandBuffer, bindingPoint, pipeline->GetPipelineLayout(), setIndex, 1, &set, 0, nullptr);
			});
	}
	void VulkanRenderCommandBuffer::PushConstants(Buffer buffer, uint32_t offset, uint32_t flags)
	{
		Ref<VulkanRenderCommandBuffer> instance = this;
		Buffer dataBuffer = Buffer::Copy(buffer);

		Renderer::Submit([instance, pipeline = m_CurrentPipeline, dataBuffer, offset, flags]() mutable {
			vkCmdPushConstants(instance->m_ActiveCommandBuffer, pipeline->GetPipelineLayout(), VK_SHADER_STAGE_ALL, offset, dataBuffer.Size, dataBuffer.Data);
		dataBuffer.Release();
			});
	}
	void VulkanRenderCommandBuffer::SetPipeline(Ref<Pipeline> pipeline)
	{
		HZR_PROFILE_FUNCTION();
		m_CurrentPipeline = pipeline.As<VulkanPipeline>();
		Ref<VulkanRenderCommandBuffer> instance = this;
		float lineWidth = pipeline->GetSpecifications().LineWidth;


		Renderer::Submit([instance, pipeline = m_CurrentPipeline, lineWidth]() mutable {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::SetPipeline");
		HZR_ASSERT(instance->m_State == State::Record, "Command buffer not in recording state");

		if (lineWidth > 1.0f)
			glLineWidth(lineWidth);

		pipeline->Bind(instance->m_ActiveCommandBuffer);
			});
	}
	void VulkanRenderCommandBuffer::Draw(size_t count, Ref<GPUBuffer> indexBuffer)
	{
		DrawInstanced(count, 1, indexBuffer);
	}
	void VulkanRenderCommandBuffer::DrawInstanced(size_t count, uint32_t instanceCount, Ref<GPUBuffer> indexBuffer)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanGPUBuffer> buffer = indexBuffer ? indexBuffer.As<VulkanGPUBuffer>() : nullptr;
		Ref<VulkanRenderCommandBuffer> instance = this;

		if (indexBuffer)
			HZR_ASSERT(buffer->GetUsageFlags() & BUFFER_USAGE_INDEX_BUFFER_BIT, "Invalid buffer flags");

		Renderer::Submit([instance, buffer, count, instanceCount, pipeline = m_CurrentPipeline]() mutable {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::DrawInstanced");
		HZR_ASSERT(instance->m_State == State::Record, "Command buffer not in recording state");
		auto vkPipeline = pipeline->GetVulkanPipeline();
		auto shader = pipeline->GetShader().As<VulkanShader>();
		auto layout = pipeline->GetPipelineLayout();

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
	void VulkanRenderCommandBuffer::DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t drawCount, uint32_t stride, uint32_t offset, Ref<GPUBuffer> indexBuffer)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanRenderCommandBuffer> instance = this;
		Ref<VulkanGPUBuffer> indexBuf = indexBuffer.As<VulkanGPUBuffer>();
		Ref<VulkanGPUBuffer> buffer = argumentBuffer.As<VulkanGPUBuffer>();

		Renderer::Submit([instance, indexBuf, buffer, drawCount, stride, offset]() mutable {

			VkBuffer vkBuffer = buffer->GetVulkanBuffer();
		if (!indexBuf)
		{
			vkCmdDrawIndirect(instance->m_ActiveCommandBuffer, vkBuffer, offset, drawCount, stride);
			return;
		}

		VkBuffer vkIndexBuffer = indexBuf->GetVulkanBuffer();

		vkCmdBindIndexBuffer(instance->m_ActiveCommandBuffer, vkIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexedIndirect(instance->m_ActiveCommandBuffer, vkBuffer, offset, drawCount, stride);
			});
	}
	void VulkanRenderCommandBuffer::DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t stride, uint32_t offset, Ref<GPUBuffer> drawCountBuffer, uint32_t drawCountOffset, uint32_t maxDraws, Ref<GPUBuffer> indexBuffer)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanRenderCommandBuffer> instance = this;
		Ref<VulkanGPUBuffer> indexBuf = indexBuffer.As<VulkanGPUBuffer>();
		Ref<VulkanGPUBuffer> buffer = argumentBuffer.As<VulkanGPUBuffer>();
		Ref<VulkanGPUBuffer> countBuffer = drawCountBuffer.As<VulkanGPUBuffer>();

		Renderer::Submit([instance, buffer, countBuffer, indexBuf, stride, offset, maxDraws, drawCountOffset]() mutable {
			VkBuffer vkBuffer = buffer->GetVulkanBuffer();
		VkBuffer count = countBuffer->GetVulkanBuffer();

		if (!indexBuf)
		{
			vkCmdDrawIndirectCount(instance->m_ActiveCommandBuffer, vkBuffer, offset, count, drawCountOffset, maxDraws, stride);
			return;
		}

		VkBuffer vkIndexBuffer = indexBuf->GetVulkanBuffer();

		vkCmdBindIndexBuffer(instance->m_ActiveCommandBuffer, vkIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexedIndirectCount(instance->m_ActiveCommandBuffer, vkBuffer, offset, count, drawCountOffset, maxDraws, stride);
			});
	}
	void VulkanRenderCommandBuffer::DispatchCompute(GroupSize globalGroupSize)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance, size = globalGroupSize]() mutable {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::DispatchCompute");
		HZR_ASSERT(instance->m_State == State::Record, "Command buffer not in recording state");

		vkCmdDispatch(instance->m_ActiveCommandBuffer, size.x, size.y, size.z);
			});
	}
	void VulkanRenderCommandBuffer::TraceRays(const TraceRaysInfo& traceRaysInfo)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance, info = traceRaysInfo]() {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::TraceRays");
		HZR_ASSERT(instance->m_State == State::Record, "Command buffer not in recording state");

		auto bindingTable = info.pBindingTable.As<VulkanShaderBindingTable>();
		auto raygenTable = bindingTable->GetRaygenTableAddress();
		auto missTable = bindingTable->GetMissTableAddress();
		auto closestHitTable = bindingTable->GetClosestHitTableAddress();
		auto callableTable = bindingTable->GetCallableTableAddress();

		fpCmdTraceRaysKHR(instance->m_ActiveCommandBuffer, &raygenTable, &missTable, &closestHitTable, &callableTable, info.Extent.Width, info.Extent.Height, info.Extent.Depth);
			});
	}
	void VulkanRenderCommandBuffer::BeginPerformanceQuery_RT()
	{
		VkCommandBuffer commandBuffer = m_ActiveCommandBuffer;
		vkCmdResetQueryPool(commandBuffer, m_TimestampQueryPools[m_FrameIndex], 0, m_TimestampQueryCount);
		vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, m_TimestampQueryPools[m_FrameIndex], 0);
		vkCmdResetQueryPool(commandBuffer, m_PipelineQueryPools[m_FrameIndex], 0, m_PipelineQueryCount);
		vkCmdBeginQuery(commandBuffer, m_PipelineQueryPools[m_FrameIndex], 0, 0);
	}
	void VulkanRenderCommandBuffer::EndPerformanceQuery_RT()
	{
		HZR_ASSERT(m_State == State::Record, "Command buffer not in recording state");

		VkCommandBuffer commandBuffer = m_ActiveCommandBuffer;
		vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, m_TimestampQueryPools[m_FrameIndex], 1);
		vkCmdEndQuery(commandBuffer, m_PipelineQueryPools[m_FrameIndex], 0);
	}
	void VulkanRenderCommandBuffer::GetQueryPoolResults_RT()
	{
		auto device = VulkanContext::GetLogicalDevice();
		vkGetQueryPoolResults(device->GetVulkanDevice(), m_TimestampQueryPools[m_FrameIndex], 0, m_TimestampNextAvailQuery, m_TimestampNextAvailQuery * sizeof(uint64_t), m_TimestampQueryResults[m_FrameIndex].data(), sizeof(uint64_t), VK_QUERY_RESULT_64_BIT);

		for (uint64_t i = 0; i < m_TimestampNextAvailQuery; i += 2)
		{
			uint64_t startTime = m_TimestampQueryResults[m_FrameIndex][i];
			uint64_t endTime = m_TimestampQueryResults[m_FrameIndex][i + 1];


			float nanos = endTime > startTime ? (endTime - startTime) * device->GetPhysicalDevice()->GetDeviceLimits().TimestampPeriod : 0.0f;
			m_GPUExecutionTimes[m_FrameIndex][i / 2] = nanos * 0.000001f;
		}
		//vkGetQueryPoolResults(device->GetVulkanDevice(), m_PipelineQueryPools[m_FrameIndex], 0, 1, sizeof(PipelineStatistic), &m_PipelineStatisticQueryResults[m_FrameIndex], sizeof(uint64_t), VK_QUERY_RESULT_64_BIT);
	}
}
#endif
