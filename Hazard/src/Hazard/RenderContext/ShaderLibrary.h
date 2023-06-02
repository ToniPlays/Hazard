#pragma once

#include "Backend/Core/Pipeline.h"

namespace Hazard
{
	class ShaderLibrary
	{
	public:
		static void Init();
		static AssetHandle GetPipelineAssetHandle(const std::string& name);

	private:
		inline static std::unordered_map<std::string, AssetHandle> s_LoadedPipelines;
	};
}
