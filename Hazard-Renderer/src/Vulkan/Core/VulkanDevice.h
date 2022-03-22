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
		VulkanDevice(VkInstance instance, VkSurfaceKHR surface, uint32_t imagesInFlight);
		~VulkanDevice();

		void WaitUntilIdle();
		inline VkDevice GetDevice() { return m_Device; }

		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		VkPipelineCache GetPipelineCache() { return m_PipelineCache; }
		VkCommandPool GetCommandPool() { return m_CommandPool; }
		VkCommandBuffer GetCommandBuffer(bool begin);

		VkCommandBuffer CreateSecondaryCommandBuffer();

		void FlushCommandBuffer(VkCommandBuffer buffer);
		void FlushGraphicsCommandBuffer(VkCommandBuffer buffer);

		VkDescriptorPool GetDescriptorPool(uint32_t index) { return m_DescriptorPools[index]; }
		QueueIndices GetGraphicsQueue() { return m_GraphicsQueue; }
		QueueIndices GetPresentQueue() { return m_PresentQueue; }
		

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		VkFormat GetDepthFormat() const { return m_DepthFormat; }

	private:
		void CreatePools(uint32_t count);

		VkDevice m_Device;
		VkPhysicalDevice m_PhysicalDevice;
		VkCommandPool m_CommandPool;
		std::vector<VkDescriptorPool> m_DescriptorPools;
		VkPipelineCache m_PipelineCache;

		QueueIndices m_GraphicsQueue;
		QueueIndices m_PresentQueue;

		VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED;
	};
}
