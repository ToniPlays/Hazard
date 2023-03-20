#pragma once

#include "Jobs.h"
#include "IAssetLoader.h"

namespace Hazard 
{
	class AssetLoader
	{
		friend class AssetManager;
	public:
		AssetLoader() = default;
		~AssetLoader() = default;

		JobPromise<bool> Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags = 0);
		JobPromise<bool> Save(Ref<Asset>& asset);

	private:
		std::unordered_map<AssetType, Scope<IAssetLoader>> m_Loaders;

	};
}
