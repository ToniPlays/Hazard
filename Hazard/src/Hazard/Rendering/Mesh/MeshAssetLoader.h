#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard 
{
	class MeshAssetLoader : public IAssetLoader {
	public:
		MeshAssetLoader() = default;
		~MeshAssetLoader() = default;

		LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset);
		bool Save(Ref<Asset>& asset);
	};
}