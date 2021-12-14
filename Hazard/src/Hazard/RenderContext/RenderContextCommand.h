#pragma once

#include "RenderContext.h"

namespace Hazard::Rendering {

	class RenderContextCommand {
		friend class RenderContext;
	public:
		static void SetTitle(const char* title);


		static float GetWindowWidth();
		static float GetWindowHeight();
		static bool IsFullscreen();
		static void SetFullscreen(bool fullscreen);

		static bool IsVsync();
		static void SetVsync(bool vsync);

		template<typename T>
		static void Submit(T&& func) 
		{
			func();
			/*auto renderCmd = [](void* ptr) {
				auto pFunc = (T*)ptr;
				(*pFunc)();


				pFunc->~T();
			};*/
		}
		static void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) { m_Context->GetContext().BeginRenderPass(buffer, renderPass); }
		static void EndRenderPass(Ref<RenderCommandBuffer> buffer) { m_Context->GetContext().EndRenderPass(buffer); }
		static void SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth) { m_Context->GetContext().SetLineWidth(buffer, lineWidth); }
		static RenderContext& GetContext() { return *m_Context; };
		static void SetDebugCallback(ErrorCallback& callback);
		static void SetClearColor(const Color& color);
		static uint32_t GetImagesInFlight() { return m_Context->GetImagesInFlight(); }

	private:
		static void Init(RenderContext* context) { m_Context = context; }
		static RenderContext* m_Context;
	};
}