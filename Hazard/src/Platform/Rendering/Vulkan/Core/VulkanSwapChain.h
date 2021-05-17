#pragma once

#include <vulkan/vulkan.h>
#include "VulkanDevice.h"
#include <GLFW/glfw3.h>

namespace Hazard::Rendering::Vulkan {
	class VulkanSwapChain {
	public:
		VulkanSwapChain(VulkanDevice* device);
		~VulkanSwapChain();


	private:
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& modes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capability, GLFWwindow* window);
	private:

		VkSwapchainKHR m_SwapChain;
		VulkanDevice* m_Device;

		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_ImageFormat;
		VkExtent2D m_extent;
	};
}