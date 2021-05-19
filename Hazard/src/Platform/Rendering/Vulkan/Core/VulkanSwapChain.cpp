#pragma once

#include "hzrpch.h"
#include "VulkanSwapChain.h"

#include "../VKUtils.h"

namespace Hazard::Rendering::Vulkan {

	VulkanSwapChain::VulkanSwapChain(VulkanDevice* device)
	{
		m_Device = device;
		SwapChainSupportDetails details = VKUtils::GetSwapChainDetails(device->GetPhysicalDevice(), VulkanInstance::GetData().Surface);
		
		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(details.formats);
		VkPresentModeKHR presentMode = ChoosePresentMode(details.presentModes);
		VkExtent2D extent = ChooseSwapExtent(details.capabilities, VulkanInstance::GetData().window);

		uint32_t imageCount = details.capabilities.minImageCount + 1;
		if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
			imageCount = details.capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = VulkanInstance::GetData().Surface->GetSurface();

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = VKUtils::GetQueueFamilyIndices(device->GetPhysicalDevice(), VulkanInstance::GetData().Surface);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };;

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = details.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		VkResult result = vkCreateSwapchainKHR(device->GetDevice(), &createInfo, nullptr, &m_Data.swapChain);

		if (result != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan SwapChain");
		}
		
		vkGetSwapchainImagesKHR(device->GetDevice(), m_Data.swapChain, &imageCount, nullptr);

		m_Data.swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device->GetDevice(), m_Data.swapChain, &imageCount, m_Data.swapChainImages.data());

		m_Data.imageFormat = surfaceFormat.format;
		m_Data.extent = extent;

		m_Data.renderPass = new VulkanRenderTexture(m_Data.imageFormat);
		m_Data.defaultPipeline = new VulkanPipeline(m_Data.renderPass->GetRenderPass(), m_Data.extent);

		HZR_CORE_INFO("Created Vulkan SwapChain");
	}
	VulkanSwapChain::~VulkanSwapChain()
	{
		vkDestroySwapchainKHR(m_Device->GetDevice(), m_Data.swapChain, nullptr);
	}
	VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
	{
		for (const auto& availableFormat : formats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
				availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return formats[0];
	}
	VkPresentModeKHR VulkanSwapChain::ChoosePresentMode(const std::vector<VkPresentModeKHR>& modes)
	{
		for (const auto& availablePresentMode : modes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}
	VkExtent2D VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capability, GLFWwindow* window)
	{
		if (capability.currentExtent.width != UINT32_MAX) {
			return capability.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::max(capability.minImageExtent.width, std::min(capability.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capability.minImageExtent.height, std::min(capability.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}
}