#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Math/Color.h"

namespace Hazard::Rendering {

	enum DrawType { Points, Line, LineLoop, LineStrip, Triangle, TriangleStrip, TriangleFan, Outline };

	struct ErrorData {
		std::string info;
		std::string errorCode;
		int severity;

		ErrorData(const char* message, const char* type) : info(message), errorCode(type), severity(0) 
		{

		}
	};

	struct DeviceSpec {
		std::string name;
		std::string renderer;
		uint32_t textureSlots;
	};
	using ErrorCallback = void(*)(ErrorData& data);
	class Window;

	class GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init(Window* window, ApplicationCreateInfo* appInfo) = 0;

		virtual void ClearFrame() const = 0;
		virtual void SetViewport(int x, int y, int w, int h) const = 0;
		virtual void SetClearColor(glm::vec4 clearColor) = 0;
		//virtual void SetDepthTest(DepthFunc type) const = 0;

		//virtual void DrawIndexed(VertexArray& array, uint32_t size, DrawType type = DrawType::Triangle) const = 0;
		virtual DeviceSpec GetDeviceSpec() const = 0;
		
		virtual void SetErrorListener(const ErrorCallback& listener) = 0;
	};
}