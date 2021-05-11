#pragma once

#include "RenderEngine.h"
#include "Hazard/RenderContext/RenderContext.h"

namespace Hazard::Rendering {

	struct RenderStats {
		uint32_t draws = 0;
		uint32_t quads = 0;
		uint32_t vertices = 0;
		uint32_t indices = 0;
	};

	class RenderCommand {
	public:
		static void Init();
		static void ResetStats();
		static RenderStats& GetStats() { return s_Stats; }

		static void Begin(Camera camera, BackgroundRenderer& renderer) { s_Engine->BeginRendering(camera, renderer); }
		static void End() { s_Engine->EndRendering(); }
		static void SetRenderTarget(Ref<RenderTexture>& texture) { s_Engine->SetRenderTarget(texture.Raw()); }

		static void DrawIndexed(VertexArray* vao, uint32_t count);

		template<typename T>
		static void Submit(T element) { s_Engine->Submit(element); }

	private:
		static RenderContext* s_Context;
		static RenderEngine* s_Engine;
		static RenderStats s_Stats;
	};
}