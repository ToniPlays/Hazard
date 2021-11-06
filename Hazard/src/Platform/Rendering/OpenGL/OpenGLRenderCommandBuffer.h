#pragma once

#include "Hazard/Rendering/RenderCommandBuffer.h"

namespace Hazard::Rendering::OpenGL {
	class OpenGLRenderCommandBuffer : public RenderCommandBuffer {
	public:
		OpenGLRenderCommandBuffer(uint32_t size, const std::string& name) {};
		OpenGLRenderCommandBuffer(const std::string& name, bool swapchain) {};
		~OpenGLRenderCommandBuffer() = default;

		void Begin() {};
		void End() {};
		void Submit() {};
	};
}