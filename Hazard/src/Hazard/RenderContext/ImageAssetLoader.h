#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard 
{
	class ImageAssetLoader : public IAssetLoader 
	{
	public:
		ImageAssetLoader() = default;
		~ImageAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset>& asset) override;
		Ref<JobGraph> Create(const std::filesystem::path& path) override;
	};
}