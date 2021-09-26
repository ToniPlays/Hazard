#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Math/Color.h"

#include "Hazard/Rendering/Buffers/VertexArray.h"

namespace Hazard::Rendering {

	enum DrawType { Points, Line, LineLoop, LineStrip, Triangle, TriangleStrip, TriangleFan, Outline };

	struct ErrorData {
		std::string info;
		std::string errorCode;
		int severity;

		ErrorData(const char* message, const char* type) : info(message), errorCode(type), severity(0) {}
	};

	struct DeviceSpec {
		std::string name = "Undefined";
		std::string renderer = "Undefined";
		uint32_t textureSlots = 0;
	};
	using ErrorCallback = void(*)(ErrorData& data);
	class Window;

	class GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init(Window* window, ApplicationCreateInfo* appInfo) = 0;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void SwapBuffers() = 0;

		virtual void SetViewport(int x, int y, int w, int h) = 0;
		virtual void SetClearColor(glm::vec4 clearColor) = 0;
		virtual DeviceSpec GetDeviceSpec() const = 0;

		virtual void DrawIndexed(VertexArray* array, uint32_t size) = 0;
		
		virtual void SetErrorListener(const ErrorCallback& listener) = 0;
	};
}