#pragma once

#include <vulkan/vulkan.h>
#include "Platform/System/Window.h"

namespace Hazard::Rendering::Vulkan 
{
	struct QueueIndices {
		VkQueue Queue;
		uint32_t Index;
	};

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
		VkCommandBuffer GetCommandBuffer(bool begin);
		void FlushCommandBuffer(VkCommandBuffer buffer);
		void FlushGraphicsCommandBuffer(VkCommandBuffer buffer);

		VkDescriptorPool GetDescriptorPool() { return m_DescriptorPool; }
		QueueIndices GetGraphicsQueue() { return m_GraphicsQueue; }
		QueueIndices GetPresentQueue() { return m_PresentQueue; }
		DeviceSpec GetSpec();

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		VkFormat GetDepthFormat() const { return m_DepthFormat; }

	private:
		void CreatePools();

		VkDevice m_Device;
		VkPhysicalDevice m_PhysicalDevice;
		VkCommandPool m_CommandPool;
		VkDescriptorPool m_DescriptorPool;
		VkPipelineCache m_PipelineCache;

		QueueIndices m_GraphicsQueue;
		QueueIndices m_PresentQueue;

		VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED;
	};
}
