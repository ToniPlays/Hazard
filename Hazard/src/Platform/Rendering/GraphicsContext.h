#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Math/Color.h"
#include "Hazard/Rendering/Buffers/VertexArray.h"



namespace Hazard::Rendering {

	struct ErrorData {
		std::string info;
		uint32_t errorCode;

		ErrorData(std::string i, uint32_t error) : info(i), errorCode(error) {};
	};

	enum DepthTest {
		Never = 0,
		Less = 1,
		Equal = 2,
		LEqual = 3,
		Greater = 4,
		NotEqual = 5,
		GEqual = 6,
		Always = 7
	};
	enum Function {
		Multisample = 0,
		CullFace,
		Depth,
		Blend
	};


	using ErrorCallback = std::function<void(ErrorData&)>;

	class  GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() const = 0;

		virtual void ClearFrame(glm::vec4 clearColor = { 0.0, 0.0, 0.0, 1.0 }) const = 0;
		virtual void SetViewport(int x, int y, int w, int h) const = 0;
		virtual void SetDepthTest(DepthTest type) const = 0;
		virtual void Enable(Function fn) const = 0;
		virtual void Disable(Function fn) const = 0;

		virtual void DrawIndexed(VertexArray* array, uint32_t size) const = 0;
		virtual std::string GetVersion() const = 0;
		virtual std::string GetDevice() const = 0;
		
		virtual void SetErrorListener(const ErrorCallback& listener) = 0;
		
		static GraphicsContext* Create(void* window, void* props);

	};
}