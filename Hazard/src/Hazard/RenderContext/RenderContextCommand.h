#pragma once

#include "RenderContext.h"
namespace Hazard::Rendering {

	class RenderContextCommand {
	public:
		static void Init();
		static bool IsVsync();
		static void SetVsync(bool vsync);
		static void ClearFrame(Color color);

		static RenderContext& GetContext() { return context; };

	private:
		static RenderContext context;
	};
}