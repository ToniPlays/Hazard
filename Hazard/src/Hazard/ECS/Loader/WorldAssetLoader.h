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
		Ref<JobGraph> FromSourceFile(const std::filesystem::path& path) override { return nullptr; };
		Ref<JobGraph> Create(const std::filesystem::path& path) override;

		Buffer ToBinary(Ref<Asset> asset) override { return Buffer(); };
	};
}