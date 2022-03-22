//#pragma once
//
//#include "Hazard/Rendering/Queue/RenderCommandBuffer.h"
//
//namespace Hazard::Rendering::OpenGL {
//	class OpenGLRenderCommandBuffer : public RenderCommandBuffer {
//	public:
//		OpenGLRenderCommandBuffer(uint32_t size, const std::string& name) {};
//		OpenGLRenderCommandBuffer(const std::string& name, bool swapchain) {};
//		~OpenGLRenderCommandBuffer() = default;
//
//		virtual uint32_t GetFrameIndex() { return 0; };
//		virtual bool IsRecording() { return true;  };
//
//		void Begin() {};
//		void End() {};
//		void Submit() {};
//	};
//}