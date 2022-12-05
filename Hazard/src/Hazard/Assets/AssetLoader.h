#pragma once
#include "IAssetLoader.h"
#include "Jobs.h"

namespace Hazard 
{
	class AssetLoader
	{
		friend class AssetManager;
	public:
	public:
		AssetLoader() = default;
		~AssetLoader() = default;

		LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags = 0);
		JobPromise LoadAsync(AssetMetadata& metadata, uint32_t flags = 0);
		bool Save(Ref<Asset>& asset);
		JobPromise SaveAsync(Ref<Asset>& asset);

	private:
		std::unordered_map<AssetType, Scope<IAssetLoader>> m_Loaders;
	};
}
