#pragma once

#include <vulkan/vulkan.h>
#include "Platform/System/Window.h"
#include "VulkanDevice.h"

namespace Hazard::Rendering::Vulkan
{

	typedef struct _SwapChainBuffers {
		VkImage image;
		VkImageView view;
	} SwapChainBuffer;

	class VulkanSwapChain {
	public:
		VulkanSwapChain() = default;
		~VulkanSwapChain() { Cleanup(); }

		void Connect(VkInstance instance, VulkanDevice* device, VkSurfaceKHR surface);
		void Create(uint32_t* width, uint32_t* height, bool vsync = false);

		uint32_t GetImageCount() { return m_ImageCount; }
		VkFormat GetColorFormat() { return m_ColorFormat; }
		SwapChainBuffer GetBuffer(uint32_t index) { return m_Buffers[index]; }
		uint32_t GetWidth() { return m_Width; }
		uint32_t GetHeight() { return m_Height; }

		VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
		VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);

		void Cleanup();

	private:
		VulkanDevice* m_Device;
		VkSurfaceKHR m_Surface;

		VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
		VkSwapchainKHR m_OldSwapChain = VK_NULL_HANDLE;

		VkFormat m_ColorFormat;
		VkColorSpaceKHR m_ColorSpace;
		VkPresentModeKHR m_PresentMode;
		uint32_t m_ImageCount;

		std::vector<VkImage> m_Images;
		std::vector<SwapChainBuffer> m_Buffers;

		uint32_t m_QueueNodeIndex;
		uint32_t m_Width = 0, m_Height = 0;


	};
}
