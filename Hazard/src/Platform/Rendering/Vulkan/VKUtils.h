#pragma once

#include <vulkan/vulkan.h>
#include "VKContext.h"
#include <optional>

namespace Hazard::Rendering::Vulkan {

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	
	
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VKUtils {
	public:
		static bool SuitableDevice(VkPhysicalDevice device, VulkanData data);
		static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

		static void CreateSwapchain(VulkanData& data);

		static SwapChainSupportDetails GetSwapchainDetails(VkPhysicalDevice device, VkSurfaceKHR surface);
		static std::vector<const char*> GetRequiredExtensions(bool validation = true);
		static QueueFamilyIndices VKFindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
		static VkPhysicalDevice GetVulkanDevice(VkInstance instance, VulkanData data);

	private:
		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
		static VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& modes);
		static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capability, GLFWwindow* window);
	};
}