#pragma once
#include <hzrpch.h>
#include "VulkanAllocator.h"
#include "Platform/Rendering/Vulkan/VKContext.h"

#include "Platform/Rendering/Vulkan/Buffers/CommandBuffer.h"
#include "Platform/Rendering/Vulkan/Device/VulkanDevice.h"
#include "Platform/Rendering/Vulkan/Device/SwapChain.h"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace Hazard::Rendering::Vulkan
{
	VulkanAllocator::VulkanAllocator(const char* name)
	{
		auto device = VKContext::GetDevice();
		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.physicalDevice = device->GetPhysicalDevice();
		allocatorInfo.device = device->GetDevice();
		allocatorInfo.instance = VKContext::GetInstance()->GetInstance();

		vmaCreateAllocator(&allocatorInfo, &m_Allocator);
	}
	VulkanAllocator::~VulkanAllocator()
	{
		vmaDestroyAllocator(m_Allocator);
	}
	VmaAllocation VulkanAllocator::AllocateBuffer(VkBufferCreateInfo* info, VmaMemoryUsage usage, VkBuffer* buffer)
	{
		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = usage;

		auto device = VKContext::GetDevice();
		VmaAllocation alloc;
		vmaCreateBuffer(m_Allocator, info, &allocInfo, buffer, &alloc, nullptr);
		return alloc;
	}
	void VulkanAllocator::UnmapMemory(VmaAllocation alloc)
	{
		vmaUnmapMemory(m_Allocator, alloc);
		vmaFreeMemory(m_Allocator, alloc);
	}
	void VulkanAllocator::CopyBufferToImage(VkBuffer buffer, VkImage image, VkExtent2D extent)
	{
		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent.width = extent.width;
		region.imageExtent.height = extent.height;
		region.imageExtent.depth = 1;

		auto cmdBuffer = VKContext::GetDevice()->BeginSingleTimeCommands();
		vkCmdCopyBufferToImage(cmdBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		VKContext::GetDevice()->EndSingleTimeCommands(cmdBuffer);

	}
	void VulkanAllocator::TransitionImageLayuout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		auto device = VKContext::GetDevice();
		VkCommandBuffer buffer = device->BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags srcStage;
		VkPipelineStageFlags destStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}

		vkCmdPipelineBarrier(buffer, srcStage, destStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		device->EndSingleTimeCommands(buffer);
	}
}
