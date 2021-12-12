#pragma once

#include "Hazard/Rendering/RenderCommandBuffer.h"

namespace Hazard::Rendering::OpenGL {
	class OpenGLRenderCommandBuffer : public RenderCommandBuffer {
	public:
		OpenGLRenderCommandBuffer(uint32_t size, const std::string& name) {};
		OpenGLRenderCommandBuffer(const std::string& name, bool swapchain) {};
		~OpenGLRenderCommandBuffer() = default;

		virtual RenderStats& GetStats() { return m_Stats; };
		virtual uint32_t GetFrameIndex() { return 0; };

		void Begin() { m_Stats.Reset(); };
		void End() {};
		void Submit() {};
	private:
		RenderStats m_Stats;
	};
}