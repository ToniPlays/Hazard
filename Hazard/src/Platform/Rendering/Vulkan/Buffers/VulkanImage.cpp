#pragma once

#include <hzrpch.h>
#include "VulkanImage.h"
#include "../Device/VulkanDevice.h"

namespace Hazard::Rendering::Vulkan 
{
	void VulkanImage::CreateImage(VulkanDevice* device, VkImage& image, VkDeviceMemory& imageMemory, VkImageLayout layout, VkImageType type, VkExtent2D extent, VkFormat depthFormat)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = type;
		imageInfo.extent.width = extent.width;
		imageInfo.extent.height = extent.height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = depthFormat;

		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = 0;

		if (vkCreateImage(device->GetDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device->GetDevice(), image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, VK_QUEUE_GRAPHICS_BIT);

		if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		if (vkBindImageMemory(device->GetDevice(), image, imageMemory, 0) != VK_SUCCESS) {
			throw std::runtime_error("failed to bind image memory!");
		}
	}
	void VulkanImage::CreateImageView(const VkDevice& device, const VkImage& image, VkImageView& view, VkImageViewType type, VkFormat format, VkImageAspectFlags flags,
		uint32_t mips, uint32_t baseMip, uint32_t layerCount, uint32_t baseLayerCount)
	{
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = type;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = flags;
		viewInfo.subresourceRange.baseMipLevel = baseMip;
		viewInfo.subresourceRange.levelCount = mips;
		viewInfo.subresourceRange.baseArrayLayer = baseLayerCount;
		viewInfo.subresourceRange.layerCount = layerCount;

		auto result = vkCreateImageView(device, &viewInfo, nullptr, &view);
		HZR_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan ImageView");
	}
}
