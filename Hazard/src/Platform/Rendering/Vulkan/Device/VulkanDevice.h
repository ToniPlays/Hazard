#pragma once

#include <vulkan/vulkan.h>
#include "../Core/Instance.h"
#include "Platform/System/Window.h"

namespace Hazard::Rendering::Vulkan 
{
	class SwapChain;
	class CommandBuffer;

	class VulkanDevice {
	public:
		VulkanDevice(Instance* instance, uint32_t imagesInFlight);
		~VulkanDevice();

		void SetClearColor(glm::vec4 clearColor);
		void WaitUntilIdle();
		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer buffer);

		CommandBuffer* CreateCommandBuffer();
		void FreeCommandBuffer(CommandBuffer* buffer);

		operator VkDevice() const { return m_Device; }
		operator VkPhysicalDevice() const { return m_PhysicalDevice; }

		inline VkDevice GetDevice() { return m_Device; }
		WindowSurface* GetDeviceSurface() { return m_Surface; }

		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		SwapChain& GetSwapChain() { return *m_SwapChain; }
		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		VkQueue GetPresentQueue() { return m_PresentQueue; }
		VkCommandPool GetCommandPool() { return m_CommandPool; }
		DeviceSpec GetSpec();

		VkFormat GetDepthFormat();
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	private:
		VkDevice m_Device;
		VkPhysicalDevice m_PhysicalDevice;

		WindowSurface* m_Surface;
		std::unique_ptr<SwapChain> m_SwapChain;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		VkCommandPool m_CommandPool;
	};
}
