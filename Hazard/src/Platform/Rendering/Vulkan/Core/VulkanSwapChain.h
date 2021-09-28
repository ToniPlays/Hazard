#pragma once

#include <vulkan/vulkan.h>
#include "Platform/System/Window.h"
#include "VulkanDevice.h"
#include "VulkanAllocator.h"

namespace Hazard::Rendering::Vulkan
{
	struct DepthStencil {
		VkImage image;
		VkImageView view;
		VkDeviceMemory mem;
	};

	struct VulkanSemaphore {
		VkSemaphore PresentComplete;
		VkSemaphore RenderComplete;
	};

	typedef struct _SwapChainBuffers {
		VkImage image;
		VkImageView view;
	} SwapChainBuffer;

	class VulkanSwapChain : public RefCount {
	public:
		VulkanSwapChain() = default;
		~VulkanSwapChain() { Cleanup(); }

		void Connect(VkInstance instance, VulkanDevice* device, VkSurfaceKHR surface);
		void CreateSwapChain(uint32_t* width, uint32_t* height, bool vsync = false);
		void SwapBuffers();

		uint32_t GetImageCount() { return m_ImageCount; }
		VkFormat GetColorFormat() { return m_ColorFormat; }

		SwapChainBuffer GetBuffer(uint32_t index) { return m_Buffers[index]; }
		VkCommandBuffer GetDrawCommandBuffer(uint32_t index) { return m_DrawCommandBuffers[index]; }
		VkCommandBuffer GetCurrentDrawCommandBuffer() { return m_DrawCommandBuffers[m_CurrentBufferIndex]; }

		VkFramebuffer GetFrameBuffer(uint32_t index) { return m_FrameBuffers[index]; }
		VkFramebuffer GetCurrentFrameBuffer() { return m_FrameBuffers[m_CurrentBufferIndex]; }

		DepthStencil GetDepthStencil() { return m_DepthStencil; }
		VkRenderPass GetRenderPass() { return m_RenderPass; }
		uint32_t GetWidth() { return m_Width; }
		uint32_t GetHeight() { return m_Height; }

		VkResult AcquireNextImage(uint32_t* imageIndex);
		VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);

		void Clear();
		void Cleanup();

	private:

		void AllocateCommandBuffers();
		void CreateDepthStencil();
		void CreateRenderPass();
		void CreateFrameBuffer();

		VulkanAllocator m_Allocator;
		VkRenderPass m_RenderPass;

		VkFormat m_DepthFormat;
		DepthStencil m_DepthStencil;

		VulkanDevice* m_Device;
		VkSurfaceKHR m_Surface;

		VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
		VkSwapchainKHR m_OldSwapChain = VK_NULL_HANDLE;

		VkFormat m_ColorFormat;
		VkColorSpaceKHR m_ColorSpace;
		VkPresentModeKHR m_PresentMode;
		uint32_t m_ImageCount;
		VulkanSemaphore m_Semaphores;
		std::vector<VkFence> m_WaitFences;
		VkSubmitInfo m_SubmitInfo;

		std::vector<VkImage> m_Images;
		std::vector<SwapChainBuffer> m_Buffers;
		std::vector<VkCommandBuffer> m_DrawCommandBuffers;
		std::vector<VkFramebuffer> m_FrameBuffers;

		uint32_t m_QueueNodeIndex;
		uint32_t m_Width = 0, m_Height = 0;
		uint32_t m_CurrentBufferIndex = 0;
	};
}
