#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Utils/Maths/Color.h"

#include "Hazard/Rendering/Buffer/VertexArray.h"

namespace Hazard {

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
		virtual void ClearFrame(Color clearColor = {34, 34, 34, 255}) const = 0;
		virtual void SetViewport(int x, int y, int w, int h) const = 0;
		virtual void DrawArray(VertexArray* array) const = 0;
		virtual std::string GetVersion() const = 0;
		
		virtual void SetErrorListener(const ErrorCallback& listener) = 0;
		
		static GraphicsContext* Create(void* window, void* props);

	};
}