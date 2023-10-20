#pragma once

#include "Hazard/Assets/Asset.h"
#include "Backend/Core/ShaderCompiler.h"

namespace Hazard
{
	class ShaderAsset : public Asset
	{
		friend class ShaderAssetLoader;

	public:
		std::vector<HazardRenderer::ShaderStageCode> ShaderCode;
	};
}