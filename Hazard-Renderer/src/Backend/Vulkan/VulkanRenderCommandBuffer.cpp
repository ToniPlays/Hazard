
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
#include "Textures/VulkanCubemapTexture.h"

#include "VkUtils.h"
#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::Vulkan
{
	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(uint32_t size, const std::string& name) : m_DebugName(std::move(name))
	{
		HZR_PROFILE_FUNCTION();
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
		HZR_PROFILE_FUNCTION();
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
		HZR_PROFILE_FUNCTION();
		if (m_OwnedBySwapchain) return;

		VkCommandPool commandPool = m_CommandPool;
		Renderer::SubmitResourceFree([commandPool]() mutable {
			auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
			vkDestroyCommandPool(device, commandPool, nullptr);
			});
	}
	void VulkanRenderCommandBuffer::Begin()
	{
		HZR_PROFILE_FUNCTION();
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
		HZR_PROFILE_FUNCTION();
		uint32_t frameIndex = VulkanContext::GetFrameIndex();

		VkCommandBuffer commandBuffer = m_ActiveCommandBuffer;
		//vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, m_TimestampQueryPools[frameIndex], 1);
		//vkCmdEndQuery(commandBuffer, m_PipelineQueryPools[frameIndex], 0);
		VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer), "Failed to end command buffer");

		m_ActiveCommandBuffer = nullptr;
	}
	void VulkanRenderCommandBuffer::Submit()
	{
		HZR_PROFILE_FUNCTION();
		if (m_OwnedBySwapchain) return;
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
		HZR_PROFILE_FUNCTION();
		Ref<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance]() {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::EndRenderPass");
			vkCmdEndRenderPass(instance->m_ActiveCommandBuffer);
			});
	}
	void VulkanRenderCommandBuffer::BindVertexBuffer(Ref<VertexBuffer> vertexBuffer, uint32_t binding)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanVertexBuffer> buffer = vertexBuffer.As<VulkanVertexBuffer>();
		Ref<VulkanRenderCommandBuffer> instance = this;

		Renderer::Submit([instance, buffer, binding]() mutable {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::BindVertexBuffer");
			VkBuffer vkBuffer = buffer->GetVulkanBuffer();
			VkDeviceSize offsets = { 0 };
			vkCmdBindVertexBuffers(instance->m_ActiveCommandBuffer, 0, 1, &vkBuffer, &offsets);
			});
	}
	void VulkanRenderCommandBuffer::BindUniformBuffer(Ref<UniformBuffer> uniformBuffer)
	{
		HZR_PROFILE_FUNCTION();
	}
	void VulkanRenderCommandBuffer::BindPipeline(Ref<Pipeline> pipeline)
	{
		HZR_PROFILE_FUNCTION();
		m_CurrentPipeline = pipeline.As<VulkanPipeline>();
		Ref<VulkanRenderCommandBuffer> instance = this;

		Renderer::Submit([instance, pipeline = m_CurrentPipeline]() mutable {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::BindPipeline");
			auto vkPipeline = pipeline->GetVulkanPipeline();
			vkCmdBindPipeline(instance->m_ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);
			});
	}
	void VulkanRenderCommandBuffer::Draw(uint32_t count, Ref<IndexBuffer> indexBuffer)
	{
		DrawInstanced(count, 1, indexBuffer);
	}
	void VulkanRenderCommandBuffer::DrawInstanced(uint32_t count, uint32_t instanceCount, Ref<IndexBuffer> indexBuffer)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanIndexBuffer> buffer = indexBuffer.As<VulkanIndexBuffer>();
		Ref<VulkanRenderCommandBuffer> instance = this;

		auto& offsets = m_CurrentPipeline->GetShader().As<VulkanShader>()->GetDynamicOffsets();

		Renderer::Submit([instance, buffer, count, instanceCount, offsets, pipeline = m_CurrentPipeline]() mutable {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::DrawInstanced");
			auto vkPipeline = pipeline->GetVulkanPipeline();
			auto& shader = pipeline->GetShader().As<VulkanShader>();
			auto layout = pipeline->GetPipelineLayout();
			auto& descriptorSets = shader->GetVulkanDescriptorSets();

			vkCmdBindDescriptorSets(instance->m_ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0,
				descriptorSets.size(), descriptorSets.data(), offsets.size(), offsets.data());

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
	void VulkanRenderCommandBuffer::DispatchCompute(const DispatchComputeInfo& computeInfo)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance, info = computeInfo]() mutable {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::DispatchCompute");

			auto& pipeline = info.Pipeline.As<VulkanPipeline>();
			auto vkPipeline = pipeline->GetVulkanPipeline();
			auto& shader = pipeline->GetShader().As<VulkanShader>();
			auto layout = pipeline->GetPipelineLayout();
			auto& descriptorSets = shader->GetVulkanDescriptorSets();

			auto& device = VulkanContext::GetInstance()->GetLogicalDevice();
			VkQueue computeQueue = device->GetComputeQueue();

			//VkCommandBuffer commandBuffer = device->GetCommandBuffer(true, true);
			VkCommandBuffer commandBuffer = device->GetCommandBuffer(true, true);

			LocalGroupSize size = info.GroupSize;

			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, vkPipeline);
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, layout, 0,
				descriptorSets.size(), descriptorSets.data(), 0, nullptr);

			vkCmdDispatch(commandBuffer, size.x, size.y, size.z);

			vkEndCommandBuffer(commandBuffer);

			if (!s_ComputeFence)
			{
				VkFenceCreateInfo fenceCreateInfo = {};
				fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

				VK_CHECK_RESULT(vkCreateFence(device->GetVulkanDevice(), &fenceCreateInfo, nullptr, &s_ComputeFence), "");
			}

			vkWaitForFences(device->GetVulkanDevice(), 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
			vkResetFences(device->GetVulkanDevice(), 1, &s_ComputeFence);

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			VK_CHECK_RESULT(vkQueueSubmit(computeQueue, 1, &submitInfo, s_ComputeFence), "");
			{
				Timer timer;
				VK_CHECK_RESULT(vkWaitForFences(device->GetVulkanDevice(), 1, &s_ComputeFence, VK_TRUE, UINT64_MAX), "");
				std::cout << fmt::format("Dispatch compute {0}, [{1}, {2}, {3}] took {4} ms", 
					pipeline->GetSpecifications().DebugName, size.x, size.y, size.z, timer.ElapsedMillis()) << std::endl;
			}
			});
	}
	void VulkanRenderCommandBuffer::InsertMemoryBarrier(const MemoryBarrierInfo& info)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanRenderCommandBuffer> instance = this;
		Ref<VulkanImage2D> image = info.Image.As<VulkanImage2D>();
		Ref<VulkanCubemapTexture> cubemap = info.Cubemap.As<VulkanCubemapTexture>();

		Renderer::Submit([instance, pipeline = m_CurrentPipeline, barrierInfo = info, image, cubemap]() mutable {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::InsertMemoryBarrier");
			auto device = VulkanContext::GetLogicalDevice();
			std::vector<VkImageMemoryBarrier> barriers;

			VkMemoryBarrier memoryBarrier = {};

			QueueFamilyIndices indices = device->GetPhysicalDevice().As<VulkanPhysicalDevice>()->GetQueueFamilyIndices();

			if (barrierInfo.Image)
			{
				VkImageMemoryBarrier& barrier = barriers.emplace_back();
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.oldLayout = image->GetImageDescriptor().imageLayout;
				barrier.newLayout = image->GetImageDescriptor().imageLayout;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.baseArrayLayer = 0;
				barrier.subresourceRange.baseMipLevel = 0;
				barrier.subresourceRange.levelCount = image->GetMipLevels();
				barrier.subresourceRange.layerCount = image->GetLayerCount();
				barrier.image = image->GetVulkanImage();

				barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.srcQueueFamilyIndex = indices.Graphics;
				barrier.dstQueueFamilyIndex = indices.Compute;
			}
			if (barrierInfo.Cubemap)
			{
				VkImageMemoryBarrier& barrier = barriers.emplace_back();
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.oldLayout = cubemap->GetImageDescriptor().imageLayout;
				barrier.newLayout = cubemap->GetImageDescriptor().imageLayout;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.baseArrayLayer = 0;
				barrier.subresourceRange.baseMipLevel = 0;
				barrier.subresourceRange.levelCount = cubemap->GetMipLevels();
				barrier.subresourceRange.layerCount = 6;
				barrier.image = cubemap->GetVulkanImage();

				barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.srcQueueFamilyIndex = indices.Graphics;
				barrier.dstQueueFamilyIndex = indices.Compute;
			}


			vkCmdPipelineBarrier(instance->m_ActiveCommandBuffer,
				VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				barriers.size(), barriers.data());
			});
	}
	void VulkanRenderCommandBuffer::TransitionImageLayout(const ImageTransitionInfo& info)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanRenderCommandBuffer> instance = this;
		Ref<VulkanCubemapTexture> cubemap = info.Cubemap.As<VulkanCubemapTexture>();
		VkImageLayout layout = cubemap->GetImageDescriptor().imageLayout;

		Renderer::Submit([instance, transitionInfo = info, cubemap, layout]() mutable {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::TransitionImageLayout");
			HZR_ASSERT(transitionInfo.Image || transitionInfo.Cubemap, "What are you doing");

			if (transitionInfo.Image)
			{
				HZR_ASSERT(false, "Not implemented");
			}

			if (transitionInfo.Cubemap)
			{
				VkImageSubresourceRange range = {};

				range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				range.baseMipLevel = 0;
				range.levelCount = cubemap->GetMipLevels();
				range.baseArrayLayer = 0;
				range.layerCount = 6;

				VkUtils::InsertImageMemoryBarrier(
					instance->m_ActiveCommandBuffer,
					cubemap->GetVulkanImage(),
					VkUtils::GetVulkanAccess(transitionInfo.SourceLayout),
					VkUtils::GetVulkanAccess(transitionInfo.DestLayout),
					VkUtils::GetVulkanImageLayout(transitionInfo.SourceLayout),
					VkUtils::GetVulkanImageLayout(transitionInfo.DestLayout),
					VkUtils::GetVulkanStage(transitionInfo.SourceLayout),
					VkUtils::GetVulkanStage(transitionInfo.DestLayout),
					range
				);
				cubemap->SetImageLayout(VkUtils::GetVulkanImageLayout(transitionInfo.DestLayout));
			}
			});
	}
	void VulkanRenderCommandBuffer::SetViewport(float x, float y, float width, float height)
	{
		HZR_PROFILE_FUNCTION();
	}
	void VulkanRenderCommandBuffer::SetLineSize(float size)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance, size]() mutable {
			HZR_PROFILE_SCOPE("VulkanRenderCommandBuffer::SetLineSize");
			vkCmdSetLineWidth(instance->m_ActiveCommandBuffer, size);
			});
	}
}
#endif