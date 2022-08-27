#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Swapchain.h"
#include "VulkanDevice.h"
#include "VulkanAllocator.h"
#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace HazardRenderer::Vulkan
{

	class VulkanSwapchain : public Swapchain {
	public:
		VulkanSwapchain();
		virtual ~VulkanSwapchain();

		void Init(VkInstance instance, Ref<VulkanDevice> device);
		void InitSurface(GLFWwindow* nativeWindow);

		void Create(uint32_t* width, uint32_t* height, bool enableVSync);
		void Destroy();

		void Resize(uint32_t width, uint32_t height) override;
		void BeginFrame() override;
		void Present() override;

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		Ref<RenderCommandBuffer> GetSwapchainBuffer() override { return nullptr; }

		Ref<RenderPass> GetRenderPass() override { return nullptr; };
		Ref<FrameBuffer> GetRenderTarget() override { return nullptr; };

		//Vulkan specific

		VkFramebuffer GetCurrentFramebuffer() { return GetFrameBuffer(m_CurrentBufferIndex); }
		VkCommandBuffer GetCurrentDrawCommandBuffer() { return GetDrawCommandBuffer(m_CurrentBufferIndex); }
		VkRenderPass GetVulkanRenderPass() const { return m_RenderPass; }
		uint32_t GetCurrentBufferIndex() const { return m_CurrentBufferIndex; }

		VkFramebuffer GetFrameBuffer(uint32_t index) 
		{
			return m_FrameBuffers[index];
		}

		VkCommandBuffer GetDrawCommandBuffer(uint32_t index) 
		{
			return m_CommandBuffers[index].CommandBuffer;
		}

	private:
		void FindImageFormatAndColorSpace();

		uint32_t AcquireSwapchainImage();

	private:
		bool m_VSync = false;
		uint32_t m_Width = 0, m_Height = 0;

		VkInstance m_Instance;
		Ref<VulkanDevice> m_Device;
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;

		uint32_t m_QueueNodeIndex;

		VkFormat m_ColorFormat;
		VkColorSpaceKHR m_ColorSpace;

		//Images
		struct SwapchainImage {
			VkImage Image = VK_NULL_HANDLE;
			VkImageView ImageView = VK_NULL_HANDLE;
		};
		std::vector<SwapchainImage> m_Images;
		uint32_t m_ImageCount;

		std::vector<VkImage> m_VulkanImages;

		struct SwapchainCommandBuffer {
			VkCommandBuffer CommandBuffer = VK_NULL_HANDLE;
			VkCommandPool CommandPool = VK_NULL_HANDLE;
		};
		std::vector<SwapchainCommandBuffer> m_CommandBuffers;

		VkRenderPass m_RenderPass;
		std::vector<VkFramebuffer> m_FrameBuffers;


		struct SwapchainSemaphores 
		{
			VkSemaphore RenderComplete = VK_NULL_HANDLE;
			VkSemaphore PresentComplete = VK_NULL_HANDLE;
		};
		SwapchainSemaphores m_Semaphores;
		std::vector<VkFence> m_WaitFences;

		VkSubmitInfo m_SubmitInfo;

		uint32_t m_CurrentImageIndex = 0;
		uint32_t m_CurrentBufferIndex = 0;
	};
}
#endif
