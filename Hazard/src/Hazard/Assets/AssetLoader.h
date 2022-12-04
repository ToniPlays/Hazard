#pragma once
#include "IAssetLoader.h"

namespace Hazard 
{
	class AssetLoader
	{
		friend class AssetManager;
	public:
	public:
		AssetLoader() = default;
		~AssetLoader() = default;

		LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset);
		JobPromise LoadAsync(AssetMetadata& metadata);
		bool Save(Ref<Asset>& asset);
		JobPromise SaveAsync(Ref<Asset>& asset);

	private:
		std::unordered_map<AssetType, Scope<IAssetLoader>> m_Loaders;
	};
}
