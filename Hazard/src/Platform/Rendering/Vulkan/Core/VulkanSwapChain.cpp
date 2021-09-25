#pragma once
#include <hzrpch.h>
#include "VulkanSwapChain.h"
#include "../VKUtils.h"

namespace Hazard::Rendering::Vulkan
{
	void VulkanSwapChain::Connect(VkInstance instance, VulkanDevice* device, VkSurfaceKHR surface)
	{
		this->m_Device = device;
		m_Surface = surface;
	}
	void VulkanSwapChain::Create(uint32_t* width, uint32_t* height, bool vsync)
	{
		m_Width = *width;
		m_Height = *height;

		m_OldSwapChain = m_SwapChain;
		VkDevice device = m_Device->GetDevice();
		VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice();

		SwapChainSupportDetails details = VKUtils::GetSwapChainDetails(m_Device->GetPhysicalDevice(), m_Surface);
		VkSurfaceFormatKHR surfaceFormat = VKUtils::ChooseSwapChainFormat(details.formats);

		m_PresentMode = VKUtils::ChooseSwapChainPresentMode(details.presentModes);
		m_ColorFormat = surfaceFormat.format;
		m_ColorSpace = surfaceFormat.colorSpace;

		m_ImageCount = details.capabilities.minImageCount + 1;

		if (details.capabilities.maxImageCount > 0 && m_ImageCount > details.capabilities.maxImageCount) {
			m_ImageCount = details.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface;
		createInfo.minImageCount = m_ImageCount;
		createInfo.imageFormat = m_ColorFormat;
		createInfo.imageColorSpace = m_ColorSpace;
		createInfo.imageExtent = { m_Width, m_Height };
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.preTransform = details.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = m_PresentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = m_OldSwapChain;

		QueueFamilyIndices indices = VKUtils::GetQueueFamilyIndices(m_Device->GetPhysicalDevice(), m_Surface);
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
		HZR_CORE_ASSERT(result == VK_SUCCESS, "Failed to create swapchain");

		vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_SwapChain, &m_ImageCount, nullptr);
		m_Images.resize(m_ImageCount);
		vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_SwapChain, &m_ImageCount, m_Images.data());

		m_Buffers.resize(m_ImageCount);

		for (uint32_t i = 0; i < m_ImageCount; i++) {
			VkImageViewCreateInfo colorAttachmentView = {};
			colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			colorAttachmentView.format = m_ColorFormat;
			colorAttachmentView.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
			colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			colorAttachmentView.subresourceRange.baseMipLevel = 0;
			colorAttachmentView.subresourceRange.levelCount = 1;
			colorAttachmentView.subresourceRange.baseArrayLayer = 0;
			colorAttachmentView.subresourceRange.layerCount = 1;
			colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
			colorAttachmentView.flags = 0;

			m_Buffers[i].image = m_Images[i];
			colorAttachmentView.image = m_Images[i];

			vkCreateImageView(device, &colorAttachmentView, nullptr, &m_Buffers[i].view);
		}
	}
	VkResult VulkanSwapChain::AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex)
	{
		return vkAcquireNextImageKHR(m_Device->GetDevice(), m_SwapChain, UINT64_MAX, presentCompleteSemaphore, (VkFence)nullptr, imageIndex);
	}
	VkResult VulkanSwapChain::QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_SwapChain;
		presentInfo.pImageIndices = &imageIndex;

		if (waitSemaphore) {
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &waitSemaphore;
		}
		return vkQueuePresentKHR(queue, &presentInfo);
	}
	void VulkanSwapChain::Cleanup()
	{
		VkDevice device = m_Device->GetDevice();

		if (m_SwapChain) {
			for (uint32_t i = 0; i < m_ImageCount; i++) {
				vkDestroyImageView(device, m_Buffers[i].view, nullptr);
			}
		}
		if (m_Surface) {
			vkDestroySwapchainKHR(device, m_SwapChain, nullptr);
		}
		m_Surface = VK_NULL_HANDLE;
		m_SwapChain = VK_NULL_HANDLE;
	}
}