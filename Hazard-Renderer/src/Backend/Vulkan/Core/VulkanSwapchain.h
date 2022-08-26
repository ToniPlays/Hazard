#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Swapchain.h"
#include "VulkanDevice.h"
#include "VulkanAllocator.h"
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	class VulkanSwapchain : public Swapchain {
	public:
		VulkanSwapchain();
		virtual ~VulkanSwapchain();

		void Create(uint32_t width, uint32_t height, bool vSync) override;
		void Resize(uint32_t width, uint32_t height) override;
		void CreateResources(FrameBufferCreateInfo* targetInfo);

		void BeginFrame() override;
		void Present() override;

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		void Cleanup();

		//void AddResizeCallback(const ResizeCallback& callback) { m_ResizeEvent.push_back(callback); }

	private:
		void CreateFramebuffer();
		void CreateDepthStencil();

	private:
		bool m_VSync = false;
		uint32_t m_Width = 0, m_Height = 0;
		VkSurfaceKHR m_Surface;

		friend class VulkanContext;
	};
}
#endif
