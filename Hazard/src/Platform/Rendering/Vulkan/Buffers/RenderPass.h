#pragma once

#include "vulkan/vulkan.h"
#include "../Device/VulkanDevice.h"

namespace Hazard::Rendering::Vulkan
{
	class RenderPass {
	public:
		RenderPass(VulkanDevice* device, VkFormat swapChainFormat);
		~RenderPass();

		void Begin(CommandBuffer* buffer, VkFramebuffer frameBuffer, VkExtent2D extent);
		void End(CommandBuffer* buffer);
		void CreateDepthImages(VulkanDevice* device, VkExtent2D extent, uint32_t count);

		VkImageView GetDepthImage(uint32_t index) { return m_DepthImageViews[index]; }
		VkRenderPass GetRenderPass() { return m_RenderPass; }

	private:
		VkDevice m_Device;
		VkRenderPass m_RenderPass;

		std::vector<VkImage> m_DepthImages;
		std::vector<VkDeviceMemory> m_DepthMemorys;
		std::vector<VkImageView> m_DepthImageViews;
	};
}