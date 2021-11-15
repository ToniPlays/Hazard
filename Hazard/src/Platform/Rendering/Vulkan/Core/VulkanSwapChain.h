#pragma once

#include "Platform/System/Window.h"
#include "VulkanDevice.h"
#include "VulkanAllocator.h"
#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan
{
	class VulkanSwapChain : public RefCount {
	public:
		VulkanSwapChain();
		virtual ~VulkanSwapChain();

		void Create(uint32_t* width, uint32_t* height, bool vSync);
		void Resize(uint32_t width, uint32_t height);

		void BeginFrame();
		void Present();

		uint32_t GetImageCount() { return m_ImageCount; }

		uint32_t GetWidth() { return m_Width; }
		uint32_t GetHeight() { return m_Height; }

		VkRenderPass GetRenderPass() { return m_RenderPass; }
		VkFramebuffer GetCurrentFrameBuffer() { return GetFrameBuffer(m_CurrentImageIndex); }
		VkCommandBuffer GetCurrentDrawCommandBuffer() { return GetCommandBuffer(m_CurrentBufferIndex); }

		VkFormat GetColorFormat() { return m_ColorFormat; }
		uint32_t GetCurrentBufferIndex() { return m_CurrentBufferIndex; }

		VkFramebuffer GetFrameBuffer(uint32_t index) {
			return m_FrameBuffers[index];
		}
		VkCommandBuffer GetCommandBuffer(uint32_t index) {
			return m_CommandBuffers[index];
		}
		VkSemaphore GetRenderCompleteSemaphore() { return m_Semaphores.RenderComplete; }
		void Cleanup();

		void AddResizeCallback(const ResizeCallback& callback) { m_ResizeEvent.push_back(callback); }

	private:
		VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
		VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);

		void CreateFramebuffer();
		void CreateDepthStencil();

	private:
		VkInstance m_Instance;
		VulkanDevice* m_Device;
		bool m_VSync = false;

		VkFormat m_ColorFormat;
		VkColorSpaceKHR m_ColorSpace;

		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
		uint32_t m_ImageCount = 0;
		std::vector<VkImage> m_Images;

		struct SwapchainBuffer {
			VkImage Image;
			VkImageView View;
		};
		std::vector<SwapchainBuffer> m_Buffers;

		struct {
			VkImage Image;
			VkImageView View;
			VmaAllocation allocation;
		} m_DepthStencil;

		std::vector<VkFramebuffer> m_FrameBuffers;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		struct
		{
			// Swap chain
			VkSemaphore PresentComplete;
			// Command buffer
			VkSemaphore RenderComplete;
		} m_Semaphores;
		VkSubmitInfo m_SubmitInfo;

		std::vector<VkFence> m_WaitFences;

		VkRenderPass m_RenderPass;
		uint32_t m_CurrentBufferIndex = 0;
		uint32_t m_CurrentImageIndex = 0;

		uint32_t m_QueueNodeIndex = UINT32_MAX;
		uint32_t m_Width = 0, m_Height = 0;

		VkSurfaceKHR m_Surface;

		std::vector<ResizeCallback> m_ResizeEvent;

		friend class VulkanContext;
	};
}
