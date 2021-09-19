#pragma once

#include <hzrpch.h>
#include "VulkanDescriptorPool.h"

namespace Hazard::Rendering::Vulkan 
{
	VulkanDescriptorPool::VulkanDescriptorPool(VulkanDevice* device, VulkanSwapChain* swapChain)
	{
		VkDescriptorPoolSize poolSize {};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = swapChain->GetImageCount();

		VkDescriptorPoolCreateInfo poolInfo {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = swapChain->GetImageCount();

		if (vkCreateDescriptorPool(device->GetDevice(), &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan DescriptorPool");
		}
	}
	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
	}
}
