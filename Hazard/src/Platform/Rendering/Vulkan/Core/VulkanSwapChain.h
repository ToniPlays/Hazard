#pragma once

#include <vulkan/vulkan.h>
#include "VulkanDevice.h"
#include "VulkanPipeline.h"
#include "../Texture/VulkanRenderTexture.h"
#include <GLFW/glfw3.h>

namespace Hazard::Rendering::Vulkan {

	struct SwapChainData {
		std::vector<VkImage> swapChainImages;

		VkFormat imageFormat;
		VkExtent2D extent;
		VkSwapchainKHR swapChain;
		VulkanRenderTexture* renderPass;

		VulkanPipeline* defaultPipeline;
	};


	class VulkanSwapChain {
	public:
		VulkanSwapChain(VulkanDevice* device);
		~VulkanSwapChain();

		SwapChainData GetData() { return m_Data; }

	private:
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& modes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capability, GLFWwindow* window);
	private:

		SwapChainData m_Data;

		
		VulkanDevice* m_Device;
	};
}