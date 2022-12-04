#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard {
	class WorldAssetLoader : public IAssetLoader 
	{
	public:
		WorldAssetLoader() = default;
		~WorldAssetLoader() = default;

		LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset) override;
		JobPromise LoadAsync(AssetMetadata& metadata) override;
		bool Save(Ref<Asset>& asset) override;
		JobPromise SaveAsync(Ref<Asset>& asset) override;
	};
}