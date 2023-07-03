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

		Ref<JobGraph> Load(AssetMetadata& metadata);
		Ref<JobGraph> Save(Ref<Asset>& asset);
		Ref<JobGraph> Create(AssetType type, const std::filesystem::path& path);

	private:
		std::unordered_map<AssetType, Scope<IAssetLoader>> m_Loaders;

	};
}
