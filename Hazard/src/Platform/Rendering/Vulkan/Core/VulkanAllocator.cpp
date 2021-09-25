#pragma once

#include <hzrpch.h>
#include "VulkanAllocator.h"
#include "../VulkanContext.h"

namespace Hazard::Rendering::Vulkan {

	VulkanAllocator::VulkanAllocator(const std::string& name) : m_Tag(name)
	{

	}
	VulkanAllocator::~VulkanAllocator()
	{

	}
	void VulkanAllocator::Allocate(VkMemoryRequirements requirements, VkDeviceMemory* dest, VkMemoryPropertyFlags flags)
	{
		auto device = VulkanContext::GetDevice();
		HZR_CORE_TRACE("[VulkanAllocator]: Allocating {0} bytes", requirements.size);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = requirements.size;
		allocInfo.memoryTypeIndex = device->FindMemoryType(requirements.memoryTypeBits, flags);

		vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, dest);
	}
}
