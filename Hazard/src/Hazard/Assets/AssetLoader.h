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
		bool Save(Ref<Asset>& asset);

	private:
		std::unordered_map<AssetType, Scope<IAssetLoader>> m_Loaders;
	};
}
