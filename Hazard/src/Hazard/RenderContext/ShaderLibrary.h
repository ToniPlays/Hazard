#pragma once

#include "Backend/Core/Pipeline/Pipeline.h"

namespace Hazard
{
	class ShaderLibrary
	{
	public:
		static void Init();

		static Ref<HazardRenderer::Pipeline> GetPipeline(const std::string& name);
		static Ref<AssetPointer> GetPipelinePtr(const std::string& name);
		

	private:
		inline static std::unordered_map<std::string, Ref<AssetPointer>> m_LoadedShaders;
	};
}