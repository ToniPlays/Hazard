
#include "VulkanRenderCommandBuffer.h"
#ifdef HZR_INCLUDE_VULKAN
#include "VulkanContext.h"
#include "Backend/Core/Renderer.h"

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
		Ref<VulkanRenderCommandBuffer> instance = this;

		Renderer::Submit([instance]() mutable {
			Ref<VulkanSwapchain> swapchain = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>();

			uint32_t frameIndex = swapchain->GetCurrentBufferIndex();

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			VkCommandBuffer buffer;
			if (instance->m_OwnedBySwapchain)
				buffer = swapchain->GetCurrentDrawCommandBuffer();
			else
				buffer = instance->m_CommandBuffers[frameIndex];

			instance->m_ActiveCommandBuffer = buffer;

			VK_CHECK_RESULT(vkBeginCommandBuffer(buffer, &beginInfo), "Failed to begin Command Buffer");

			//vkCmdResetQueryPool(buffer, instance->m_TimestampQueryPools[frameIndex], 0, instance->m_TimestampQueryCount);
			//vkCmdWriteTimestamp(buffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, instance->m_TimestampQueryPools[frameIndex], 0);

			//vkCmdResetQueryPool(buffer, instance->m_PipelineQueryPools[frameIndex], 0, instance->m_PipelineQueryCount);
			//vkCmdBeginQuery(buffer, instance->m_PipelineQueryPools[frameIndex], 0, 0);

			});
	}
	void VulkanRenderCommandBuffer::End()
	{
		Ref<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance]() mutable {
			uint32_t frameIndex = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>()->GetCurrentBufferIndex();

			VkCommandBuffer commandBuffer = instance->m_ActiveCommandBuffer;
			//vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, instance->m_TimestampQueryPools[frameIndex], 1);
			//vkCmdEndQuery(commandBuffer, instance->m_PipelineQueryPools[frameIndex], 0);
			VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer), "Failed to end command buffer");

			instance->m_ActiveCommandBuffer = nullptr;

			});
	}
	void VulkanRenderCommandBuffer::Submit()
	{
		if (m_OwnedBySwapchain) return;
	}
}
#endif