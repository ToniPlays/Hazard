#pragma once

#include <vulkan/vulkan.h>
#include "Platform/System/Window.h"

namespace Hazard::Rendering::Vulkan 
{
	class SwapChain;
	class CommandBuffer;

	class VulkanDevice {
	public:
		VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
		~VulkanDevice();

		void WaitUntilIdle();
		inline VkDevice GetDevice() { return m_Device; }

		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		VkPipelineCache GetPipelineCache() { return m_PipelineCache; }
		VkCommandPool GetCommandPool() { return m_CommandPool; }
		VkDescriptorPool GetDescriptorPool() { return m_DescriptorPool; }
		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		VkQueue GetPresentQueue() { return m_PresentQueue; }
		DeviceSpec GetSpec();

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	private:

		void CreatePools();

		VkDevice m_Device;
		VkPhysicalDevice m_PhysicalDevice;
		VkCommandPool m_CommandPool;
		VkDescriptorPool m_DescriptorPool;
		VkPipelineCache m_PipelineCache;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
	};
}
