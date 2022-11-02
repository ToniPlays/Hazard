#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard
{
	class ShaderAssetLoader : public IAssetLoader
	{
	public:
		ShaderAssetLoader() = default;
		~ShaderAssetLoader() = default;

		LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset);
		bool Save(Ref<Asset>& asset);
	};
}