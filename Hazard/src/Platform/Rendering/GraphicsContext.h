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

	using ErrorCallback = std::function<void(ErrorData&)>;

	class HAZARD_API GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() const = 0;
		virtual void ClearFrame(Color clearColor = { 0, 0, 0, 255 }) const = 0;
		virtual void SetViewport(int x, int y, int w, int h) const = 0;
		virtual void DrawIndexed(VertexArray* array, uint32_t size) const = 0;
		virtual std::string GetVersion() const = 0;
		
		virtual void SetErrorListener(const ErrorCallback& listener) = 0;
		
		static GraphicsContext* Create(void* window, void* props);

	};
}