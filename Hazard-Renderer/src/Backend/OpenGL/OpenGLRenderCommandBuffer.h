#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/RenderCommandBuffer.h"
#include "Backend/Core/Swapchain.h"


namespace HazardRenderer::OpenGL 
{
	class OpenGLRenderCommandBuffer : public RenderCommandBuffer {
	public:
		OpenGLRenderCommandBuffer(uint32_t size, const std::string& name);
		OpenGLRenderCommandBuffer(const std::string& name, bool swapchain);
		~OpenGLRenderCommandBuffer() = default;

		virtual uint32_t GetFrameIndex() { return m_FrameIndex; };
		virtual bool IsRecording() { return true;  };

		void Begin();
		void End() {};
		void Submit() {};

		virtual void BeginRenderPass(Ref<RenderPass> renderPass, bool explicitClear = false);
		virtual void BeginRenderPass_RT(Ref<RenderPass> renderPass, bool explicitClear = false);
		virtual void EndRenderPass();
		virtual void EndRenderPass_RT();

		virtual void BindPipeline(Ref<Pipeline> pipeline) {};
		virtual void SetViewport(float x, float y, float width, float height) {};
		virtual void SetLineSize(float size) {};

	private:
		uint32_t m_FrameIndex = 0;
		Ref<Swapchain> m_Swapchain;
	};
}
#endif
