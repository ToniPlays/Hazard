#pragma once

#include "../Core/VulkanDevice.h"

namespace Hazard::Rendering::Vulkan
{
	class VulkanCommandPool {
	public:
		VulkanCommandPool(VulkanDevice* device);
		~VulkanCommandPool() = default;

		VkCommandPool GetPool() { return m_CommandPool; }
	private:
		VkCommandPool m_CommandPool;
	};
}