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

		JobPromise<Ref<Asset>> Load(AssetMetadata& metadata, bool synchronous = false);
		JobPromise<bool> Save(Ref<Asset>& asset);

	private:
		std::unordered_map<AssetType, Scope<IAssetLoader>> m_Loaders;

	};
}
