#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "Core/Rendering/Image2D.h"

namespace Hazard 
{
	struct ImageAssetLoaderSettings
	{
		uint32_t Resolution = 8192;
		bool FlipOnLoad = true;
	};

	struct ImageAssetFileHeader
	{
		HazardRenderer::Extent Extent;
		HazardRenderer::ImageFormat Format;
	};

	class ImageAssetLoader : public IAssetLoader 
	{
	public:
		ImageAssetLoader() = default;
		~ImageAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
		Ref<JobGraph> Create(const CreateAssetSettings& settings) override;

	private:
		static void ImageDataLoadFromSource(JobInfo& info, const std::filesystem::path& path, ImageAssetLoaderSettings settings);
		static void CreateImageFromData(JobInfo& info, ImageAssetLoaderSettings settings);

		static void ReadImageDataFromGPU(JobInfo& info, Ref<HazardRenderer::Image2D> image);
		static void GenerateImageBinary(JobInfo& info, Ref<HazardRenderer::Image2D> image);
		static void CreateImageFromBinary(JobInfo& info, AssetHandle handle);
	};
}
