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

		LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags = 0);
		Ref<JobGraph> LoadAsync(AssetMetadata& metadata, uint32_t flags = 0);
		bool Save(Ref<Asset>& asset);
		Ref<JobGraph> SaveAsync(Ref<Asset>& asset);

	private:
		std::unordered_map<AssetType, Scope<IAssetLoader>> m_Loaders;
	};
}
