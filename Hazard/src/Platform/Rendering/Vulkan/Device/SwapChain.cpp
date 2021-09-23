#pragma once
#include <hzrpch.h>
#include "SwapChain.h"
#include "../VKUtils.h"
#include "../Pipeline/Pipeline.h"
#include "../Buffers/VulkanImage.h"
#include "../Buffers/CommandBuffer.h"

namespace Hazard::Rendering::Vulkan
{
	SwapChain::SwapChain(VulkanDevice& device, uint32_t maxImagesInFlight, bool vsync)
	{
		m_Device = &device;
		m_MaxImagesInFlight = maxImagesInFlight;

		m_PresentMode = vsync ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_MAILBOX_KHR;

		CreateSwapChain();

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < m_MaxImagesInFlight; i++) {
			if (vkCreateSemaphore(m_Device->GetDevice(), &semaphoreInfo, nullptr, &m_ImageSemaphoresAvail[i]) !=
				VK_SUCCESS ||
				vkCreateSemaphore(m_Device->GetDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) !=
				VK_SUCCESS ||
				vkCreateFence(m_Device->GetDevice(), &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS) {
				HZR_THROW("failed to create synchronization objects for frame!");
			}
		}
	}

	SwapChain::~SwapChain()
	{
		
	}
	VkResult SwapChain::AcquireNextImage(uint32_t* index)
	{
		vkWaitForFences(m_Device->GetDevice(), 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);
		return vkAcquireNextImageKHR(m_Device->GetDevice(), m_SwapChain, UINT64_MAX, m_ImageSemaphoresAvail[m_CurrentFrame], VK_NULL_HANDLE, index);
	}
	void SwapChain::ResizeSwapChain(int w, int h)
	{
		m_Device->WaitUntilIdle();
		CleanUp();
		CreateSwapChain();
	}
	VkResult SwapChain::SubmitCommandBuffer(CommandBuffer* buffer, uint32_t* index)
	{
		if (m_ImagesInFlight[*index] != VK_NULL_HANDLE) {
			vkWaitForFences(m_Device->GetDevice(), 1, &m_ImagesInFlight[*index], VK_TRUE, UINT64_MAX);
		}
		m_ImagesInFlight[m_CurrentFrame] = m_InFlightFences[m_CurrentFrame];

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_ImageSemaphoresAvail[m_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = buffer->GetBuffer();

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		SubmitFrame(&submitInfo);
		return Present(signalSemaphores, &m_CurrentFrame);
		
	}
	void SwapChain::CreateSwapChain()
	{
		SwapChainSupportDetails details = VKUtils::GetSwapChainDetails(m_Device->GetPhysicalDevice(), m_Device->GetDeviceSurface()->GetVkSurface());
		m_SurfaceFormat = VKUtils::ChooseSwapChainFormat(details.formats);
		m_PresentMode = VKUtils::ChooseSwapChainPresentMode(details.presentModes, m_PresentMode);
		m_Extent = VKUtils::ChooseSwapChainExtent(details.capabilities, 1280, 720);

		uint32_t imageCount = details.capabilities.minImageCount + 1;
		if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount) {
			imageCount = details.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Device->GetDeviceSurface()->GetVkSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = m_SurfaceFormat.format;
		createInfo.imageColorSpace = m_SurfaceFormat.colorSpace;
		createInfo.imageExtent = m_Extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.preTransform = details.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = m_PresentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = m_OldSwapChain == nullptr ? VK_NULL_HANDLE : m_OldSwapChain->m_SwapChain;

		QueueFamilyIndices indices = VKUtils::GetQueueFamilyIndices(m_Device->GetPhysicalDevice(), m_Device->GetDeviceSurface()->GetVkSurface());
		uint32_t families[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = families;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}
		auto result = vkCreateSwapchainKHR(m_Device->GetDevice(), &createInfo, nullptr, &m_SwapChain);
		HZR_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan SwapChain");

		vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_SwapChain, &m_ImageCount, nullptr);
		m_SwapChainImages.resize(m_ImageCount);
		vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_SwapChain, &m_ImageCount, m_SwapChainImages.data());
		
		m_SwapChainImageViews.resize(m_ImageCount);

		for (uint32_t i = 0; i < m_ImageCount; i++) {
			m_SwapChainImageViews[i] = VulkanImage::CreateImageView(m_SwapChainImages[i], VK_IMAGE_VIEW_TYPE_2D, m_SurfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
		}
		m_RenderPass = std::make_unique<RenderPass>(m_Device, m_SurfaceFormat.format);
		m_RenderPass->CreateDepthImages(m_Device, m_Extent, m_ImageCount);

		m_SwapChainFrameBuffers.resize(m_ImageCount);
		for (size_t i = 0; i < m_ImageCount; i++)
		{
			std::vector<VkImageView> attachments = { m_SwapChainImageViews[i], m_RenderPass->GetDepthImage(i) };
			m_SwapChainFrameBuffers[i] = new FrameBuffer(m_Device, m_RenderPass.get(), attachments, m_Extent);
		}

		m_ImageSemaphoresAvail.resize(m_MaxImagesInFlight);
		m_RenderFinishedSemaphores.resize(m_MaxImagesInFlight);
		m_InFlightFences.resize(m_MaxImagesInFlight);
		m_ImagesInFlight.resize(m_ImageCount, VK_NULL_HANDLE);

		m_Buffers.resize(m_ImageCount);
		for (size_t i = 0; i < m_Buffers.size(); i++) 
		{
			m_Buffers[i] = CommandBuffer::Create();
		}

	}
	void SwapChain::CleanUp()
	{
		for (auto frameBuffer : m_SwapChainFrameBuffers) {
			delete frameBuffer;
		}
		for (auto commandBuffer : m_Buffers) {
			CommandBuffer::Free(commandBuffer);
			delete commandBuffer;
		}
		m_RenderPass.reset();

		for (auto view : m_SwapChainImageViews) {
			vkDestroyImageView(m_Device->GetDevice(), view, nullptr);
		}
		vkDestroySwapchainKHR(m_Device->GetDevice(), m_SwapChain, nullptr);
	}
	VkResult SwapChain::SubmitFrame(VkSubmitInfo* info)
	{
		vkResetFences(m_Device->GetDevice(), 1, &m_InFlightFences[m_CurrentFrame]);

		if (vkQueueSubmit(m_Device->GetGraphicsQueue(), 1, info, m_InFlightFences[m_CurrentFrame]) !=
			VK_SUCCESS) {
			HZR_THROW("failed to submit draw command buffer!");
		}
	}
	VkResult SwapChain::Present(VkSemaphore* signalSemaphores, uint32_t* index)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_SwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = index;

		m_CurrentFrame = (m_CurrentFrame + 1) % m_MaxImagesInFlight;
		return vkQueuePresentKHR(m_Device->GetPresentQueue(), &presentInfo);
	}
}