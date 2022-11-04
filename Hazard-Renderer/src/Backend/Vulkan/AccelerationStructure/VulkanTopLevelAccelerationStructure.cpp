

#include "VulkanTopLevelAccelerationStructure.h"

#include <vulkan/vulkan.h>


namespace HazardRenderer::Vulkan
{
	VulkanTopLevelAccelerationStructure::VulkanTopLevelAccelerationStructure(TopLevelAccelerationStructureCreateInfo* info)
	{
		VkAccelerationStructureCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
		createInfo.createFlags = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
	}
}
