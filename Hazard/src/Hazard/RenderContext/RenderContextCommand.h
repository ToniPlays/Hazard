#pragma once

#include "RenderContext.h"

namespace Hazard::Rendering {

	class RenderContextCommand {
	public:
		static void Init();
		static void SetDepthTest(DepthTest type);

		static bool IsVsync();
		static void SetVsync(bool vsync);
		static void ClearFrame(glm::vec4 color);

		static RenderContext& GetContext() { return *m_Context; };
		static void SetDebugCallback(ErrorCallback& callback);

	private:
		static RenderContext* m_Context;
	};
}