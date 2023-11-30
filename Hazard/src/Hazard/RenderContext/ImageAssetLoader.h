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
		Ref<JobGraph> DataFromSource(const std::filesystem::path& path) override;
		Ref<JobGraph> Create(const std::filesystem::path& base, const std::filesystem::path& internalPath) override;

		Buffer ToBinary(Ref<Asset> asset) override;

	private:
		static void LoadImageJob(JobInfo& info, AssetHandle handle);
		static void LoadImageFromSourceJob(JobInfo& info, std::filesystem::path& path);
	};
}
