#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/PhysicalDevice.h"
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
		
		VulkanDevice() = default;
		VulkanDevice(VkInstance instance, VkSurfaceKHR surface, uint32_t imagesInFlight);
		~VulkanDevice();

		std::string GetDeviceName() override { return m_DeviceName; };
		const PhysicalDeviceLimits& GetDeviceLimits() const override { return m_Limits; }

		void WaitUntilIdle();

		//Vulkan methods
		
		inline VkDevice GetVulkanDevice() { return m_Device; }
		inline VkPhysicalDevice GetVulkanPhysicalDevice() { return m_PhysicalDevice; }

		VkCommandPool GetCommandPool() { return m_CommandPool; }
		QueueIndices GetGraphicsQueue() { return m_GraphicsQueue; }
		QueueIndices GetPresentQueue() { return m_PresentQueue; }

		VkCommandBuffer GetCommandBuffer(bool begin);
		void FlushCommandBuffer(VkCommandBuffer buffer);
		void FlushGraphicsCommandBuffer(VkCommandBuffer buffer);

		VkDescriptorPool GetDescriptorPool(uint32_t index) { return m_DescriptorPools[index]; }
		VkPipelineCache GetPipelineCache() { return m_PipelineCache; }

		VkCommandBuffer CreateSecondaryCommandBuffer();

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		VkFormat GetDepthFormat() const { return m_DepthFormat; }

	private:
		void CreatePools(uint32_t count);

	private:

		std::string m_DeviceName;
		PhysicalDeviceLimits m_Limits;

		VkDevice m_Device = VK_NULL_HANDLE;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
		std::vector<VkDescriptorPool> m_DescriptorPools;
		VkPipelineCache m_PipelineCache;

		QueueIndices m_GraphicsQueue;
		QueueIndices m_PresentQueue;

		VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED;
	};
}
#endif
