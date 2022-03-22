#pragma once

#include "Core/Core.h"
#include "Core/PhysicalDevice.h"
#include <vector>
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	struct QueueIndices {
		VkQueue Queue;
		uint32_t Index;
	};

	class SwapChain;
	class CommandBuffer;

	class VulkanDevice : public PhysicalDevice {
	public:
		
		VulkanDevice() {};
		VulkanDevice(VkInstance instance, VkSurfaceKHR surface, uint32_t imagesInFlight);
		~VulkanDevice();

		std::string GetDeviceName() override { return m_DeviceName; };
		const PhysicalDeviceLimits& GetDeviceLimits() const override { return m_Limits; }

		void WaitUntilIdle();
		/*
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

		*/

	private:

		std::string m_DeviceName;
		PhysicalDeviceLimits m_Limits;

		//void CreatePools(uint32_t count);

		VkDevice m_Device;
		VkPhysicalDevice m_PhysicalDevice;
		//VkCommandPool m_CommandPool;
		//std::vector<VkDescriptorPool> m_DescriptorPools;
		VkPipelineCache m_PipelineCache;

		QueueIndices m_GraphicsQueue;
		QueueIndices m_PresentQueue;

		VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED;
	};
}
