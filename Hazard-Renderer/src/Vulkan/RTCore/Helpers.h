#pragma once

#ifdef HZR_INCLUDE_VULKAN
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	struct AccelerationStructureBufferInfo
	{
		VkBuffer Buffer = VK_NULL_HANDLE;
		VmaAllocation Allocation = VK_NULL_HANDLE;
		VkDeviceAddress Address;
	};
	struct VulkanAccelStruct
	{
		VkAccelerationStructureKHR AccelerationStructure = VK_NULL_HANDLE;
		VkDeviceAddress Address = 0;
	};
}
#endif
