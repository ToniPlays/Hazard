#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard {
	class MeshAssetLoader : public IAssetLoader {
	public:
		MeshAssetLoader() = default;
		~MeshAssetLoader() = default;

		bool Load(AssetMetadata& path, Ref<Asset>& asset);
		bool Save(Ref<Asset>& asset);
	};
}