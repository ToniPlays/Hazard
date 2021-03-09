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
		template<typename T>
		static void Submit(T element) { engine.Submit(element); }

	private:
		static RenderEngine engine;
		static RenderStats stats;
	};
}