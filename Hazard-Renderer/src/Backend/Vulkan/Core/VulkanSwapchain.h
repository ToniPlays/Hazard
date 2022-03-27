#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "VulkanDevice.h"
#include "VulkanAllocator.h"
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	class VulkanSwapchain : public RefCount {
	public:
		VulkanSwapchain();
		virtual ~VulkanSwapchain();

		void Create(uint32_t width, uint32_t height, bool vSync);
		void Resize(uint32_t width, uint32_t height);

		void BeginFrame();
		void Present();

		uint32_t GetImageCount() const { return m_ImageCount; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		VkRenderPass GetRenderPass() const { return m_RenderPass; }
		VkFramebuffer GetCurrentFrameBuffer() const { return GetFrameBuffer(m_CurrentImageIndex); }
		VkCommandBuffer GetCurrentDrawCommandBuffer() const { return GetCommandBuffer(m_CurrentBufferIndex); }

		VkFormat GetColorFormat() const { return m_ColorFormat; }
		uint32_t GetCurrentBufferIndex() const { return m_CurrentBufferIndex; }

		VkFramebuffer GetFrameBuffer(uint32_t index) const {
			return m_FrameBuffers[index];
		}
		VkCommandBuffer GetCommandBuffer(uint32_t index) const {
			return m_CommandBuffers[index];
		}
		VkSemaphore GetRenderCompleteSemaphore() const { return m_Semaphores.RenderComplete; }
		void Cleanup();

		//void AddResizeCallback(const ResizeCallback& callback) { m_ResizeEvent.push_back(callback); }

	private:
		VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
		VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);

		void CreateFramebuffer();
		void CreateDepthStencil();

	private:

		VkInstance m_Instance;
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

		friend class VulkanContext;
	};
}
#endif
