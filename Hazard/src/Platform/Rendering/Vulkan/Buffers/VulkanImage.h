#pragma once

#include "vulkan/vulkan.h"



namespace Hazard::Rendering::Vulkan
{
	class VulkanDevice;
	class VulkanImage {
	public:
		static void CreateImage(VulkanDevice* device, VkImage& image, VkDeviceMemory& imageMemory, VkImageLayout layout, VkImageType type, VkExtent2D extent, VkFormat depthFormat);
		static void CreateImageView(const VkDevice& device, const VkImage& image, VkImageView& view, VkImageViewType type, VkFormat format, VkImageAspectFlags flags,
			uint32_t mips = 1, uint32_t baseMip = 0, uint32_t layerCount = 1, uint32_t baseLayerCount = 0);
	};
}