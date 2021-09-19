#pragma once
#include <hzrpch.h>
#include "VKUtils.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <set>

namespace Hazard::Rendering::Vulkan {

	std::vector<const char*> VKUtils::GetRequiredExtensions(bool validation)
	{
		uint32_t count = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + count);

		if (validation)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}
	VkPhysicalDevice VKUtils::GetVulkanCapableDevice(VulkanInstance& instance)
	{
		uint32_t deviceCount = 0;
		VkInstance vkInstnce = instance.GetData().Instance;
		vkEnumeratePhysicalDevices(vkInstnce, &deviceCount, nullptr);

		if (deviceCount == 0) 
			return VK_NULL_HANDLE;

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vkInstnce, &deviceCount, devices.data());

		VkPhysicalDevice resultDevice;

		for (const auto& device : devices) {
			if (SuitableDevice(device, instance)) {
				resultDevice = device;
				return resultDevice;
			}
		}
		HZR_THROW("Failed to find Vulkan capable device");
	}
	bool VKUtils::SuitableDevice(VkPhysicalDevice device, VulkanInstance& instance)
	{
		QueueFamilyIndices indices = GetQueueFamilyIndices(device, instance.GetData().Surface);

		bool extensionSupported = CheckDeviceExtensionSupport(device);
		bool swapchainAdequate = false;

		if (extensionSupported) {

			SwapChainSupportDetails details = GetSwapChainDetails(device, instance.GetData().Surface);
			swapchainAdequate = !details.formats.empty() && !details.presentModes.empty();
		}

		return indices.isComplete() && extensionSupported && swapchainAdequate;
	}

	bool VKUtils::CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	SwapChainSupportDetails VKUtils::GetSwapChainDetails(VkPhysicalDevice device, VulkanWindowSurface* surface)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface->GetSurface(), &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface->GetSurface(), &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface->GetSurface(), &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface->GetSurface(), &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface->GetSurface(), &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR VKUtils::ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& formats, VkFormat format, VkColorSpaceKHR space)
	{
		for (const auto& f : formats) {
			if (f.format == format && f.colorSpace == space) 
				return f;
		}
		return formats[0];
	}

	VkPresentModeKHR VKUtils::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR preferred, VkPresentModeKHR defaultMode)
	{
		for (const auto& mode : modes) {
			if (mode == preferred) 
				return mode;
		}
		return defaultMode;
	}

	VkExtent2D VKUtils::ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities, int w, int h)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
			return capabilities.currentExtent;
		VkExtent2D actualExtent = { static_cast<uint32_t>(w),
									static_cast<uint32_t>(h) };
		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.width = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}

	QueueFamilyIndices VKUtils::GetQueueFamilyIndices(VkPhysicalDevice device, VulkanWindowSurface* surface)
	{
		QueueFamilyIndices indices = {};

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface->GetSurface(), &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}
		
		return indices;
	}
}