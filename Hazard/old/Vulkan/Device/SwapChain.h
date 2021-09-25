#pragma once

#include "VulkanDevice.h"

namespace Hazard::Rendering::Vulkan
{
	class Pipeline;

	class SwapChain {
	public:
		SwapChain(VulkanDevice& device, uint32_t maxImagesInFlight, bool vsync);
		~SwapChain();


		void ResizeSwapChain(int w, int h);
		VkResult SubmitCommandBuffer(CommandBuffer* buffer, uint32_t* index);

		RenderPass* GetRenderPass() { return m_RenderPass.get(); }
		CommandBuffer& GetCurrentBuffer() { return *m_Buffers[m_CurrentFrame]; }
		VulkanFrameBuffer* GetCurrentFrameBuffer() { return m_SwapChainFrameBuffers[m_CurrentFrame]; }
		VkResult AcquireNextImage(uint32_t* index);
		uint32_t GetImageCount() { return m_ImageCount; }

		void SetClearColor(glm::vec4 clearColor) {
			this->clearColor = clearColor;
			ResizeSwapChain(m_Extent.width, m_Extent.height);
		}
		VkExtent2D GetExtent() { return m_Extent; }

	private:
		void CreateSwapChain();
		void CleanUp();

		VkResult SubmitFrame(VkSubmitInfo* info);
		VkResult Present(VkSemaphore* signalSemaphores, uint32_t* index);

	private:
		glm::vec4 clearColor;
		uint32_t m_ImageCount = 0;
		VulkanDevice* m_Device;
		VkSwapchainKHR m_SwapChain;

		std::shared_ptr<SwapChain> m_OldSwapChain = nullptr;
		std::unique_ptr<RenderPass> m_RenderPass;

		std::vector<VkImage> m_SwapChainImages;
		std::vector<VkImageView> m_SwapChainImageViews;
		std::vector<CommandBuffer*> m_Buffers;
		
		std::vector<VkFence> m_InFlightFences;
		std::vector<VkFence> m_ImagesInFlight;
		std::vector<VkSemaphore> m_ImageSemaphoresAvail;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VulkanFrameBuffer*> m_SwapChainFrameBuffers;

		VkSurfaceFormatKHR m_SurfaceFormat;
		VkPresentModeKHR m_PresentMode;
		VkExtent2D m_Extent;

		uint32_t m_CurrentFrame = 0;
		uint32_t m_MaxImagesInFlight = 1;

	};
}