#pragma once

#include "RenderContext.h"

namespace Hazard::Rendering {

	class RenderContextCommand {
	public:
		static void Init();
		//static void SetDepthTest(DepthFunc type);

		static void SetTitle(const char* title);

		static bool IsVsync();
		static void SetVsync(bool vsync);
		static void ClearFrame();

		static RenderContext& GetContext() { return *m_Context; };
		static void SetDebugCallback(ErrorCallback& callback);
		static void SetClearColor(Color color);

	private:
		static RenderContext* m_Context;
	};
}