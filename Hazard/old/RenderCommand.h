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

		static void Begin(const Camera& camera, ECS::World& world) { s_Engine->BeginRendering(camera, world); }
		static void End() { s_Engine->EndRendering(); }

		static void SetFrameBuffer(FrameBuffer* texture) { s_Engine->SetFrameBuffer(texture); }

		static void DrawIndexed(VertexArray* vao, uint32_t count);
		static void DrawGizmo(VertexArray* vao, uint32_t count);
		static void DrawOutline(VertexArray* vao, uint32_t count);

		static void ResetStats();
		static RenderStats& GetStats() { return s_Stats; }

		template<typename T>
		static void Submit(T element) { s_Engine->Submit(element); }
	private:

		static RenderContext* s_Context;
		static RenderEngine* s_Engine;
		static RenderStats s_Stats;
	};
}