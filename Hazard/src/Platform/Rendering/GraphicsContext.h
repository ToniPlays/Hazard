#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Math/Color.h"
#include "Hazard/Rendering/Buffers/VertexArray.h"
#include "Hazard/Rendering/Pipeline/GraphicsPipeline.h"

namespace Hazard::Rendering {

	struct ErrorData {
		std::string info;
		std::string errorCode;
		int severity;

		ErrorData(const char* message, const char* type) {
			info = message;
			errorCode = type;
			severity = 0;
		}
	};

	struct DeviceSpec {
		std::string name;
		std::string renderer;
		uint32_t textureSlots;
	};
	enum Function {
		Multisample = 0,
		CullFace,
		Depth,
		Blend
	};
	using ErrorCallback = void(*)(ErrorData& data);
	class Window;

	class GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init(Window* window, ApplicationCreateInfo* appInfo) = 0;

		virtual void ClearFrame(glm::vec4 clearColor = { 0.0, 0.0, 0.0, 1.0 }) const = 0;
		virtual void SetViewport(int x, int y, int w, int h) const = 0;
		virtual void SetDepthTest(DepthFunc type) const = 0;
		virtual void Enable(Function fn) const = 0;
		virtual void Disable(Function fn) const = 0;

		virtual void DrawIndexed(VertexArray& array, uint32_t size) const = 0;
		virtual DeviceSpec GetDeviceSpec() const = 0;
		
		virtual void SetErrorListener(const ErrorCallback& listener) = 0;
	};
}