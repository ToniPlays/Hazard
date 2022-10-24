#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard {
	class WorldAssetLoader : public IAssetLoader 
	{
	public:
		WorldAssetLoader() = default;
		~WorldAssetLoader() = default;

		bool Load(AssetMetadata& metadata, Ref<Asset>& asset);
		bool Save(Ref<Asset>& asset);
	};
}