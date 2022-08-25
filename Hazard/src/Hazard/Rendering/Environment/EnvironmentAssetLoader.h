#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard {
	class EnvironmentAssetLoader : public IAssetLoader {
	public:
		EnvironmentAssetLoader() = default;
		~EnvironmentAssetLoader() = default;

		bool Load(AssetMetadata& metadata, Ref<Asset>& asset);
		bool Save(Ref<Asset>& asset);
	};
}