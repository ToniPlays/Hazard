#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "Core/Rendering/Image2D.h"

namespace Hazard 
{
	

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

		Ref<JobGraph> Load(AssetMetadata& metadata, const LoadAssetSettings& settings) override;
		Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
		Ref<JobGraph> Create(const CreateAssetSettings& settings) override;

		struct CreateSettings
		{
			uint32_t Resolution = 8192;
			bool FlipOnLoad = true;
		};

	private:
		static void ImageDataLoadFromSource(JobInfo& info, const std::filesystem::path& path, CreateSettings settings);
		static void CreateImageFromData(JobInfo& info, CreateSettings settings);

		static void ReadImageDataFromGPU(JobInfo& info, Ref<HazardRenderer::Image2D> image);
		static void GenerateImageBinary(JobInfo& info, Ref<HazardRenderer::Image2D> image);
		static void CreateImageFromBinary(JobInfo& info, AssetHandle handle);
	};
}
