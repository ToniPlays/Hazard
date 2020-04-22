#pragma once
#include "Core/Core.h"

namespace Hazard {
	class HAZARD_API GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void ClearColor(Color color) = 0;
		virtual void SwapBuffers() = 0;
		virtual void SetViewport(int x, int y, int w, int h) = 0;
	};
}