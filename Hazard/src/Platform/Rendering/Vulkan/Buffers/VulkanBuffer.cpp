#pragma once
#include <hzrpch.h>
#include "VulkanBuffer.h"

#include "vulkan/vulkan.h"

namespace Hazard::Rendering::Vulkan {

	VulkanBuffer::VulkanBuffer(VulkanDevice* device, VulkanCommandPool* commandPool)
	{
		VkCommandBufferAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = commandPool->GetPool();
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(device->GetDevice(), &info, &m_Buffer) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan CommandBuffer");
		}
	}
}