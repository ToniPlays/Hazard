#pragma once
#include <hzrpch.h>
#include "VulkanSwapChain.h"
#include "../VKUtils.h"

namespace Hazard::Rendering::Vulkan
{
	VulkanSwapChain::VulkanSwapChain(VulkanDevice* device)
	{
		VulkanWindowSurface* surface = VulkanInstance::GetData().Surface;
		SwapChainSupportDetails details = VKUtils::GetSwapChainDetails(device->GetPhysicalDevice(), surface);
		VkSurfaceFormatKHR format = VKUtils::ChooseSwapChainFormat(details.formats, VK_FORMAT_B8G8R8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
		VkPresentModeKHR mode = VKUtils::ChooseSwapChainPresentMode(details.presentModes, VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_KHR);

		int w, h;
		glfwGetFramebufferSize(VulkanInstance::GetData().window, &w, &h);
		VkExtent2D extent = VKUtils::ChooseSwapChainExtent(details.capabilities, w, h);

		uint32_t imageCount = details.capabilities.minImageCount + 1;
		if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
			imageCount = details.capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface->GetSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = format.format;
		createInfo.imageColorSpace = format.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

		QueueFamilyIndices indices = VKUtils::GetQueueFamilyIndices(device->GetPhysicalDevice(), surface);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}

		createInfo.preTransform = details.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = mode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(device->GetDevice(), &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan SwapChain");
		}
		vkGetSwapchainImagesKHR(device->GetDevice(), m_SwapChain, &imageCount, nullptr);
		m_Images.resize(imageCount);
		vkGetSwapchainImagesKHR(device->GetDevice(), m_SwapChain, &imageCount, m_Images.data());
		
		m_ImageFormat = format.format;
		m_Extent = extent;

		CreateImageViews(device);
	}
	VulkanSwapChain::~VulkanSwapChain()
	{
		for (VkImageView v : m_ImageViews) 
		{
		}
	}
	void VulkanSwapChain::CreateImageViews(VulkanDevice* device)
	{
		m_ImageViews.reserve(m_Images.size());
		for (size_t i = 0; i < m_Images.size(); i++) {
			
			VkImageViewCreateInfo createInfo = {};
			createInfo.image = m_Images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_ImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device->GetDevice(), &createInfo, nullptr, &m_ImageViews[i]) != VK_SUCCESS) {
				HZR_THROW("Failed to create Vulkan ImageView");
			}
		}
	}
	void VulkanSwapChain::CreateFrameBuffers(VulkanDevice* device, VulkanRenderPass* renderPass)
	{
		m_FrameBuffers.reserve(m_ImageViews.size());
		for (size_t i = 0; i < m_ImageViews.size(); i++) {
			VkImageView attachments[] = { m_ImageViews[i] };

			VkFramebufferCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			info.renderPass = renderPass->GetRenderPass();
			info.attachmentCount = 1;
			info.pAttachments = attachments;
			info.width = m_Extent.width;
			info.height = m_Extent.height;
			info.layers = 1;

			if (vkCreateFramebuffer(device->GetDevice(), &info, nullptr, &m_FrameBuffers[i]) != VK_SUCCESS) {
				HZR_THROW("Failed to create Vulkan FrameBuffer");
			}
		}
	}
}