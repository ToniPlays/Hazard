#pragma once

#include <hzrpch.h>
#include "VulkanRenderCommandBuffer.h"
#include "VulkanContext.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "VKUtils.h"

namespace Hazard::Rendering::Vulkan 
{
	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(uint32_t size, const std::string& name)
	{
		auto device = VulkanContext::GetDevice();
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

	}
	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(const std::string& name, bool swapchain) : m_DebugName(std::move(name)), m_OwnedBySwapchain(swapchain)
	{
		auto device = VulkanContext::GetDevice();
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
	}
	VulkanRenderCommandBuffer::~VulkanRenderCommandBuffer()
	{
		if (!m_OwnedBySwapchain) {
			auto device = VulkanContext::GetDevice()->GetDevice();
			vkDestroyCommandPool(device, m_CommandPool, nullptr);
		}
	}
	void VulkanRenderCommandBuffer::Begin()
	{
		m_FrameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		beginInfo.pNext = nullptr;

		VkCommandBuffer commandBuffer = m_CommandBuffers[m_FrameIndex];
		HZR_CORE_ASSERT(commandBuffer != VK_NULL_HANDLE, "VkCommandBuffer failed");
		VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo));
	}

	void VulkanRenderCommandBuffer::End()
	{
		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
		VkCommandBuffer commandBuffer = m_CommandBuffers[frameIndex];
		VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));
	}

	void VulkanRenderCommandBuffer::Submit()
	{
		if (m_OwnedBySwapchain) return;

		auto device = VulkanContext::GetDevice();
		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[frameIndex];

		VK_CHECK_RESULT(vkWaitForFences(device->GetDevice(), 1, &m_WaitFences[frameIndex], VK_TRUE, UINT64_MAX));
		VK_CHECK_RESULT(vkResetFences(device->GetDevice(), 1, &m_WaitFences[frameIndex]));
		VK_CHECK_RESULT(vkQueueSubmit(device->GetGraphicsQueue().Queue, 1, &submitInfo, m_WaitFences[frameIndex]));
	}
}
