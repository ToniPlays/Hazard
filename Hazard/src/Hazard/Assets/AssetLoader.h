#pragma once
#include "IAssetLoader.h"

namespace Hazard 
{
	class AssetLoader
	{
	public:
	public:
		AssetLoader() = default;
		~AssetLoader() = default;

		template<typename T>
		void RegisterLoader(AssetType type)
		{
			m_Loaders[type] = CreateScope<T>();
		};
		bool Load(AssetMetadata& metadata, Ref<Asset>& asset);
		bool Save(Ref<Asset>& asset);

	private:
		std::unordered_map<AssetType, Scope<IAssetLoader>> m_Loaders;
	};
}
