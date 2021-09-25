#pragma once

#include "RenderContext.h"

namespace Hazard::Rendering {

	class RenderContextCommand {
		friend class RenderContext;
	public:
		//static void SetDepthTest(DepthFunc type);

		static void SetTitle(const char* title);

		static bool IsVsync();
		static void SetVsync(bool vsync);
		static void SwapBuffers();

		template<typename T>
		static void Submit(T func) {
			m_Context->GetContext().Submit(func);
		}

		static void DrawIndexed(VertexArray* vertexArray, uint32_t size) { m_Context->GetContext().DrawIndexed(vertexArray, size); }

		static RenderContext& GetContext() { return *m_Context; };
		static void SetDebugCallback(ErrorCallback& callback);
		static void SetClearColor(Color color);

	private:
		static void Init(RenderContext* context) { m_Context = context; }

		static RenderContext* m_Context;
	};
}