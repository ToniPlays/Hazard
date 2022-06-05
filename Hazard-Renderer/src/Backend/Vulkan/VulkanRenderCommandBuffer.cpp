
#include "VulkanRenderCommandBuffer.h"
#ifdef HZR_INCLUDE_VULKAN
#include "VulkanContext.h"
#include "VKUtils.h"

namespace HazardRenderer::Vulkan
{
	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(uint32_t size, const std::string& name)
	{
		m_DebugName = name;

		/*auto device = VulkanContext::GetDevice();
		uint32_t framesInFlight = RenderContextCommand::GetImagesInFlight();
		uint32_t count = size == 0 ? framesInFlight : size;

		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = device->GetGraphicsQueue().Index;
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		VK_CHECK_RESULT(vkCreateCommandPool(device->GetDevice(), &cmdPoolInfo, nullptr, &m_CommandPool));

		VkCommandBufferAllocateInfo cmdBufferAllocateInfo = {};
		cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdBufferAllocateInfo.commandPool = m_CommandPool;
		cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdBufferAllocateInfo.commandBufferCount = count;

		m_CommandBuffers.resize(count);
		VK_CHECK_RESULT(vkAllocateCommandBuffers(device->GetDevice(), &cmdBufferAllocateInfo, m_CommandBuffers.data()));

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		m_WaitFences.resize(count);
		for (auto& fence : m_WaitFences)
			VK_CHECK_RESULT(vkCreateFence(device->GetDevice(), &fenceCreateInfo, nullptr, &fence));
			*/

	}
	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(const std::string& name, bool swapchain) : m_DebugName(std::move(name)), m_OwnedBySwapchain(swapchain)
	{
		VulkanDevice& device = VulkanContext::GetPhysicalDevice();
		uint32_t framesInFlight = VulkanContext::GetVulkanSwapchain()->GetImageCount();

		m_CommandBuffers.resize(framesInFlight);
		Ref<VulkanSwapchain> swapChain = VulkanContext::GetVulkanSwapchain();

		for (uint32_t frame = 0; frame < framesInFlight; frame++) 
		{
			m_CommandBuffers[frame] = swapChain->GetCommandBuffer(frame);
		}
	}
	VulkanRenderCommandBuffer::~VulkanRenderCommandBuffer()
	{
		/*if (!m_OwnedBySwapchain) {
			auto device = VulkanContext::GetDevice()->GetDevice();
			vkDestroyCommandPool(device, m_CommandPool, nullptr);

			for (auto& fence : m_WaitFences) {
				vkDestroyFence(device, fence, nullptr);
			}
		}
		*/
	}
	void VulkanRenderCommandBuffer::Begin()
	{
		SetState(State::Record);
		m_FrameIndex = VulkanContext::GetVulkanSwapchain()->GetCurrentBufferIndex();

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		beginInfo.pNext = nullptr;

		VkCommandBuffer commandBuffer = GetBuffer(m_FrameIndex);
		HZR_ASSERT(commandBuffer != VK_NULL_HANDLE, "VkCommandBuffer failed");
		VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo));
	}

	void VulkanRenderCommandBuffer::End()
	{
		SetState(State::Finished);

		uint32_t frameIndex = VulkanContext::GetVulkanSwapchain()->GetCurrentBufferIndex();
		VkCommandBuffer commandBuffer = GetBuffer(frameIndex);

		VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));
	}

	void VulkanRenderCommandBuffer::Submit()
	{
		if (m_OwnedBySwapchain) return;

		Ref<VulkanRenderCommandBuffer> instance = this;
		instance->SetState(State::Submit);

		/*uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
		auto device = VulkanContext::GetDevice();

		VkCommandBuffer cmdBuffer = instance->GetBuffer(frameIndex);
		VkFence fence = instance->GetFence(frameIndex);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuffer;

		VK_CHECK_RESULT(vkWaitForFences(device->GetDevice(), 1, &fence, VK_TRUE, UINT64_MAX));
		VK_CHECK_RESULT(vkResetFences(device->GetDevice(), 1, &fence));
		VK_CHECK_RESULT(vkQueueSubmit(device->GetGraphicsQueue().Queue, 1, &submitInfo, fence));
		*/
	}
}
#endif