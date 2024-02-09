#pragma once

#include "Core/Rendering/Pipeline.h"

namespace Hazard
{
	class ShaderLibrary
	{
	public:
		static void Init(HazardRenderer::RenderAPI api);
		static Ref<HazardRenderer::Pipeline> GetPipeline(const std::string& name);

	private:
		inline static std::unordered_map<std::string, Ref<HazardRenderer::Pipeline>> s_LoadedPipelines;
	};
}
