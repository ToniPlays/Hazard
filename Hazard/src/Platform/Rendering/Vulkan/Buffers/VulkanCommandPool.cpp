#pragma once
#include <hzrpch.h>
#include "VulkanCommandPool.h"
#include "../VKUtils.h"

namespace Hazard::Rendering::Vulkan {

	VulkanCommandPool::VulkanCommandPool(VulkanDevice* device)
	{
		QueueFamilyIndices indices = VKUtils::GetQueueFamilyIndices(device->GetPhysicalDevice(), VulkanInstance::GetData().Surface);
		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.queueFamilyIndex = indices.graphicsFamily.value();

		if (vkCreateCommandPool(device->GetDevice(), &createInfo, nullptr, &m_CommandPool) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan CommandPool");
		}
	}
}