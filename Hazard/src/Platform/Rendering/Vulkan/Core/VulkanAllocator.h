#pragma once

#include "Hazard/Core/Core.h"
#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan
{
	class VulkanAllocator {
	public:
		VulkanAllocator() = default;
		VulkanAllocator(const std::string& name);
		~VulkanAllocator();

		void Allocate(VkMemoryRequirements requirements, VkDeviceMemory* dest, VkMemoryPropertyFlags flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	private:
		std::string m_Tag;
	};
}