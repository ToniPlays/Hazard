
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
		/*auto device = VulkanContext::GetDevice();
		uint32_t framesInFlight = RenderContextCommand::GetImagesInFlight();

		m_CommandBuffers.resize(framesInFlight);
		Ref<VulkanSwapChain> swapChain = VulkanContext::GetSwapchain();

		for (uint32_t frame = 0; frame < framesInFlight; frame++) {
			m_CommandBuffers[frame] = swapChain->GetCommandBuffer(frame);
		}

		swapChain->AddResizeCallback([&](uint32_t width, uint32_t height)
			{
				uint32_t framesInFlight = RenderContextCommand::GetImagesInFlight();
				Ref<VulkanSwapChain> sw = VulkanContext::GetSwapchain();
				m_CommandBuffers.resize(framesInFlight);

				for (uint32_t frame = 0; frame < framesInFlight; frame++) {
					m_CommandBuffers[frame] = sw->GetCommandBuffer(frame);
				}
			});
			*/
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
		/*m_FrameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();

		Ref<VulkanRenderCommandBuffer> instance = this;

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			beginInfo.pNext = nullptr;

			VkCommandBuffer commandBuffer = instance->GetBuffer(instance->GetFrameIndex());
			HZR_CORE_ASSERT(commandBuffer != VK_NULL_HANDLE, "VkCommandBuffer failed");
			VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo));

			*/
	}

	void VulkanRenderCommandBuffer::End()
	{
		SetState(State::Finished);
		/*Ref<VulkanRenderCommandBuffer> instance = this;

		RenderContextCommand::Submit([instance]() mutable {
			HZR_PROFILE_FUNCTION("Hazard::Rendering::VulkanRenderCommandBuffer::End() RT");

			uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
			VkCommandBuffer commandBuffer = instance->GetBuffer(frameIndex);
			VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));
			});
			*/
	}

	void VulkanRenderCommandBuffer::Submit()
	{
		if (m_OwnedBySwapchain) return;

		Ref<VulkanRenderCommandBuffer> instance = this;

		/*RenderContextCommand::Submit([instance]() mutable {

			HZR_PROFILE_FUNCTION("Hazard::Rendering::VulkanRenderCommandBuffer::Submit() RT");
			instance->SetState(State::Submit);

			uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
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
			});
			*/
	}
}
#endif