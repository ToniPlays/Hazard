#pragma once

#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan 
{
	class VulkanDescriptorPool {
	public:
		VulkanDescriptorPool();
		~VulkanDescriptorPool();

		VkDescriptorPool GetPool() { return m_DescriptorPool; }

	private:
		VkDescriptorPool m_DescriptorPool;
	};
}