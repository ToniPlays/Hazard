#pragma once

#include "Core/VulkanInstance.h"
#include "Core/VulkanWindowSurface.h"
#include <optional>

namespace Hazard::Rendering::Vulkan {

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily = 0;
		std::optional<uint32_t> presentFamily = 0;

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
		static std::vector<const char*> GetRequiredExtensions(bool validation = false);
		static VkPhysicalDevice GetVulkanCapableDevice(VulkanInstance& instance);
		static QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice device, VulkanWindowSurface* surface);
		static SwapChainSupportDetails GetSwapChainDetails(VkPhysicalDevice device, VulkanWindowSurface* surface);
		static VkSurfaceFormatKHR ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& formats, VkFormat format, VkColorSpaceKHR space);
		static VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR preferred, VkPresentModeKHR defaultMode);
		static VkExtent2D ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities, int w, int h);

	private:
		static bool SuitableDevice(VkPhysicalDevice device, VulkanInstance& instance);
		static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	};
}