#pragma once

#include "vulkan/vulkan.h"


namespace Hazard::Rendering::Vulkan {
	class VulkanDevice;

	class VulkanImage 
	{
	public:
		static VkImage CreateImage(VkDeviceMemory& imageMemory, VkImageLayout layout, VkImageUsageFlags usage, VkImageType type, VkExtent2D extent, VkFormat format);
		static VkImageView CreateImageView(const VkImage& image, VkImageViewType type, VkFormat format, VkImageAspectFlags flags,
			uint32_t mips = 1, uint32_t baseMip = 0, uint32_t layerCount = 1, uint32_t baseLayerCount = 0);
		static VkSampler CreateSampler();
	};
}