#pragma once

#include "VulkanCommandPool.h"
#include "../Core/VulkanDevice.h"

namespace Hazard::Rendering::Vulkan
{
	class VulkanBuffer {
	public:
		VulkanBuffer(VulkanDevice* device, VulkanCommandPool* commandPool);
		~VulkanBuffer() = default;

		VkCommandBuffer GetBuffer() { return m_Buffer; }
	private:
		VkCommandBuffer m_Buffer;
	};
}