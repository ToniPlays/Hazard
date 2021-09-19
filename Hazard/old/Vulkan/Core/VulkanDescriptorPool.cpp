#pragma once

#include <hzrpch.h>
#include "VulkanDescriptorPool.h"
#include "VulkanSwapChain.h"
#include "VulkanInstance.h"

namespace Hazard::Rendering::Vulkan {

	VulkanDescriptorPool::VulkanDescriptorPool()
	{
		uint32_t imageCount = VulkanInstance::GetData().SwapChain->GetData().swapChainImages.size();

		VkDescriptorPoolSize poolSize = {};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = imageCount;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = static_cast<uint32_t>(imageCount);

		if (vkCreateDescriptorPool(VulkanInstance::GetData().Device->GetDevice(), &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan descriptor pool");
		}

	}
	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		vkDestroyDescriptorPool(VulkanInstance::GetData().Device->GetDevice(), m_DescriptorPool, nullptr);

	}
}