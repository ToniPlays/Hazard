#pragma once

#include "RenderEngine.h"
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
		static RenderStats& GetStats() { return stats; }

		static void Begin(Camera camera) { engine.BeginRendering(camera); }
		static void End() { engine.EndRendering(); }
		static void SetRenderTarget(RenderTexture* texture) { engine.SetRenderTarget(texture); }

		static void DrawIndexed(VertexArray* vao, uint32_t count);

		template<typename T>
		static void Submit(T element) { engine.Submit(element); }

	private:
		static RenderContext context;
		static RenderEngine engine;
		static RenderStats stats;
	};
}