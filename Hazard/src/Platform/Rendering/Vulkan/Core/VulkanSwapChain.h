#pragma once

#include "VulkanDevice.h"
#include "VulkanWindowSurface.h"
#include "VulkanRenderPass.h"
#include "vulkan/vulkan.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Hazard::Rendering::Vulkan
{
	class VulkanSwapChain {
	public:
		VulkanSwapChain(VulkanDevice* device);
		~VulkanSwapChain();

		void CreateImageViews(VulkanDevice* device);
		void CreateFrameBuffers(VulkanDevice* device, VulkanRenderPass* renderPass);

		VkFormat GetImageFormat() { return m_ImageFormat; }
		uint32_t GetImageCount() { return m_Images.size(); }

	private:
		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;
		std::vector<VkFramebuffer> m_FrameBuffers;
		VkFormat m_ImageFormat;
		VkExtent2D m_Extent;
	};
}