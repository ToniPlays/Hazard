#pragma once

#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan {
	class VulkanDevice {
	public:
		VulkanDevice();
		~VulkanDevice();

		VkDevice GetDevice() { return m_Device; }
		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }

	private:
		VkDevice m_Device;
		VkPhysicalDevice m_PhysicalDevice;
		VkQueue m_GraphicsQueue;
	};
}
