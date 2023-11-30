#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard {
	class WorldAssetLoader : public IAssetLoader 
	{
	public:
		WorldAssetLoader() = default;
		~WorldAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset>& asset) override;
		Ref<JobGraph> DataFromSource(const std::filesystem::path& path) override { return nullptr; };
		Ref<JobGraph> Create(const std::filesystem::path& base, const std::filesystem::path& internalPath) override;

		Buffer ToBinary(Ref<Asset> asset) override { return Buffer(); };
	};
}
