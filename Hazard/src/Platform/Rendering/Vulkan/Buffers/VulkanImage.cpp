#pragma once

#include <hzrpch.h>
#include "VulkanImage.h"
#include "../Device/VulkanDevice.h"
#include "Platform/Rendering/Vulkan/VKContext.h"

namespace Hazard::Rendering::Vulkan 
{
	VkImage VulkanImage::CreateImage(VkDeviceMemory& imageMemory, VkImageLayout layout, VkImageUsageFlags usage, VkImageType type, VkExtent2D extent, VkFormat format)
	{
		auto device = VKContext::GetDevice();
		VkImage image;

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = type;
		imageInfo.extent.width = extent.width;
		imageInfo.extent.height = extent.height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;

		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = layout;
		imageInfo.usage = usage;
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

		return image;

	}
	VkImageView VulkanImage::CreateImageView(const VkImage& image, VkImageViewType type, VkFormat format, VkImageAspectFlags flags,
		uint32_t mips, uint32_t baseMip, uint32_t layerCount, uint32_t baseLayerCount)
	{
		auto device = VKContext::GetDevice();

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

		VkImageView view;

		auto result = vkCreateImageView(device->GetDevice(), &viewInfo, nullptr, &view);
		HZR_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan ImageView");

		return view;
	}
	VkSampler VulkanImage::CreateSampler()
	{
		auto device = VKContext::GetDevice();

		VkSampler sampler;
		VkPhysicalDeviceProperties props = {};
		vkGetPhysicalDeviceProperties(device->GetPhysicalDevice(), &props);

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = props.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		
		auto result = vkCreateSampler(device->GetDevice(), &samplerInfo, nullptr, &sampler);
		HZR_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan Sampler");
		return sampler;
	}
}
