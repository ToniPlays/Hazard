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
		void CreateSwapchain(uint32_t imagesInFlight, bool vSync);

		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer buffer);

		inline VkDevice GetDevice() { return m_Device; }

		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		VkCommandPool GetCommandPool() { return m_CommandPool; }
		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		VkQueue GetPresentQueue() { return m_PresentQueue; }
		DeviceSpec GetSpec();

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	private:

		void CreateCommandPool();

		VkDevice m_Device;
		VkPhysicalDevice m_PhysicalDevice;
		VkCommandPool m_CommandPool;

		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
	};
}
