#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Math/Color.h"
#include "Hazard/Rendering/RenderCommandBuffer.h"
#include "Hazard/Rendering/Pipeline/FrameBuffer.h"

namespace Hazard::Rendering {

	enum DrawType { Points, Line, LineLoop, LineStrip, Triangle, TriangleStrip, TriangleFan, Outline };

	struct ErrorData {
		std::string Info;
		std::string ErrorCode;
		int Severity;

		ErrorData(const char* message, const char* type) : Info(message), ErrorCode(type), Severity(0) {}
	};

	struct DeviceSpec {
		std::string Name = "Undefined";
		std::string Renderer = "Undefined";
		uint32_t TextureSlots = 0;
	};
	using ErrorCallback = void(*)(ErrorData& data);
	class Window;
	struct WindowProps;

	class GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init(Window* window, ApplicationCreateInfo* appInfo) = 0;
		virtual void SwapBuffers() = 0;
		virtual void Begin() = 0;
		virtual void End() = 0;

		virtual void SetViewport(int x, int y, int w, int h) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual DeviceSpec GetDeviceSpec() const = 0;

		virtual void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) {};
		virtual void EndRenderPass(Ref<RenderCommandBuffer> buffer) {};

		//virtual void DrawIndexed(VertexArray* array, uint32_t size) = 0;
		
		virtual void SetErrorListener(const ErrorCallback& listener) = 0;

		static GraphicsContext* Create(RenderAPI API, WindowProps* props);
	};
}