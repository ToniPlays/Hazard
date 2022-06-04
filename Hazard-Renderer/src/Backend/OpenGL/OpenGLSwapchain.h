#pragma once

#include "Backend/Core/Swapchain.h"
#include "OpenGLContext.h"

namespace HazardRenderer::OpenGL 
{
	class OpenGLSwapchain : public Swapchain 
	{
	public:
		OpenGLSwapchain() = default;
		OpenGLSwapchain(OpenGLContext* context, FrameBufferCreateInfo* targetInfo);

		void Create(uint32_t width, uint32_t height, bool vSync) override;
		void Resize(uint32_t width, uint32_t height) override;

		void BeginFrame() override;
		void Present() override;

		uint32_t GetWidth() const override { return m_Width; };
		uint32_t GetHeight() const override { return m_Height; };

		Ref<RenderCommandBuffer> GetSwapchainBuffer() override { return m_RenderCommandBuffer; }

		Ref<RenderPass> GetRenderPass() override { return m_RenderPass; };
		Ref<FrameBuffer> GetRenderTarget() override { return m_FrameBuffer; };

	private:
		OpenGLContext* m_Context;
		Ref<RenderCommandBuffer> m_RenderCommandBuffer;

		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		bool m_Vsync = true;

		Ref<RenderPass> m_RenderPass;
		Ref<FrameBuffer> m_FrameBuffer;
	};
}