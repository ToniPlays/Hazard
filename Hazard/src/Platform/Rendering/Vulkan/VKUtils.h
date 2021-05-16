#pragma once

#include "Core/VKInstance.h"
#include "Core/VKWindowSurface.h"
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
		static VkPhysicalDevice GetVulkanCapableDevice(VKInstance& instance);
		static QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice device, VKWindowSurface* surface);
		static SwapChainSupportDetails GetSwapChainDetails(VkPhysicalDevice device, VKWindowSurface* surface);

	private:
		static bool SuitableDevice(VkPhysicalDevice device, VKInstance& instance);
		static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	};
}