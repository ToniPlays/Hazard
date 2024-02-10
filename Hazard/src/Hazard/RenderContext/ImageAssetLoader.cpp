
#include <hzrpch.h>
#include "ImageAssetLoader.h"
#include "Hazard/Assets/AssetPack.h"
#include "TextureFactory.h"
#include "Texture2D.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Rendering/RenderEngine.h"



#include "Hazard/Core/Application.h"

namespace Hazard
{
	Ref<JobGraph> ImageAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();

		Ref<Job> createJob = Ref<Job>::Create(fmt::format("ImageLoad: {}", metadata.FilePath.string()), CreateImageFromBinary, metadata.Handle);

		JobGraphInfo info = {};
		info.Name = "Image load";
		info.Stages = { { "Create", 1.0f, { createJob } } };
		info.Flags = JOB_GRAPH_TERMINATE_ON_ERROR;

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> ImageAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		HZR_PROFILE_FUNCTION();
		HZR_CORE_ASSERT(settings.Flags & ASSET_MANAGER_COMBINE_ASSET, "Cannot override image source file");

		Ref<Job> readbackJob = Ref<Job>::Create("Readback", ReadImageDataFromGPU, asset.As<Texture2DAsset>()->GetSourceImage());
		Ref<Job> processJob = Ref<Job>::Create("Process", GenerateImageBinary, asset.As<Texture2DAsset>()->GetSourceImage());

		JobGraphInfo info = {};
		info.Name = "Image save";
		info.Stages = { { "Readback", 0.5f, { readbackJob } },
					   { "Process", 0.5f, { processJob } }
		};
		info.Flags = JOB_GRAPH_TERMINATE_ON_ERROR;

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> ImageAssetLoader::Create(const CreateAssetSettings& settings)
	{
		auto& file = settings.SourcePath;

		ImageAssetLoaderSettings imageSpec = {};
		if (settings.Settings)
			imageSpec = *(ImageAssetLoaderSettings*)settings.Settings;

		Ref<Job> sourceLoad = Ref<Job>::Create(fmt::format("Image data load from: {0}", file.string()), ImageDataLoadFromSource, file, imageSpec);
		Ref<Job> createImage = Ref<Job>::Create(fmt::format("Image {}", File::GetName(file)), CreateImageFromData, imageSpec);

		JobGraphInfo info = {};
		info.Name = "Image load";
		info.Stages = { { "Load data", 0.8f, { sourceLoad } },
						{ "Create", 0.2f, { createImage } }
		};
		info.Flags = JOB_GRAPH_TERMINATE_ON_ERROR;

		return Ref<JobGraph>::Create(info);
	}

	void ImageAssetLoader::ImageDataLoadFromSource(JobInfo& info, const std::filesystem::path& path, ImageAssetLoaderSettings settings)
	{
		if (!File::Exists(path))
			throw JobException(fmt::format("Image source file does not exist: {}", path.string()));

		TextureHeader header = TextureFactory::LoadTextureFromSourceFile(path, settings.FlipOnLoad);
		if (!header.ImageData.Data)
			throw JobException("Image load from source failed");

		info.Job->SetResult(header);
	}

	void ImageAssetLoader::CreateImageFromData(JobInfo& info, ImageAssetLoaderSettings settings)
	{
		TextureHeader header = info.ParentGraph->GetResult<TextureHeader>();

		Ref<Texture2DAsset> asset = Ref<Texture2DAsset>::Create();
		asset->SetExtent(header.Extent);
		asset->SetMaxMipLevels(header.Mips);
		asset->Invalidate(header.ImageData);

		info.Job->SetResult(asset);
	}

	void ImageAssetLoader::ReadImageDataFromGPU(JobInfo& info, Ref<HazardRenderer::Image2D> image)
	{
		using namespace HazardRenderer;

		ImageCopyRegion region = {};
		region.Extent = image->GetExtent();
		region.X = 0;
		region.Y = 0;
		region.Z = 0;

		BufferCreateInfo bufferInfo = {};
		bufferInfo.Name = "Image readback";
		bufferInfo.Size = region.Extent.Width * region.Extent.Height * region.Extent.Depth * sizeof(float);
		bufferInfo.UsageFlags = BUFFER_USAGE_STORAGE_BUFFER_BIT | BUFFER_USAGE_DYNAMIC;

		Ref<GPUBuffer> readbackBuffer = GPUBuffer::Create(&bufferInfo);

		ImageMemoryInfo barrier = {};
		barrier.Image = (Image*)image.Raw();
		barrier.BaseLayer = 0;
		barrier.LayerCount = 1;
		barrier.BaseMip = 0;
		barrier.MipCount = 1;
		barrier.SrcLayout = IMAGE_LAYOUT_SHADER_READ_ONLY;
		barrier.DstLayout = IMAGE_LAYOUT_TRANSFER_SRC;

		Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::Create("Image readback", DeviceQueue::TransferBit, 1);
		cmdBuffer->Begin();
		cmdBuffer->ImageMemoryBarrier(barrier);
		cmdBuffer->CopyImageToBuffer(image, readbackBuffer, region);

		barrier.SrcLayout = IMAGE_LAYOUT_TRANSFER_SRC;
		barrier.DstLayout = IMAGE_LAYOUT_SHADER_READ_ONLY;

		cmdBuffer->ImageMemoryBarrier(barrier);
		cmdBuffer->End();
		cmdBuffer->Submit();
		cmdBuffer->OnCompleted([info, readbackBuffer]() mutable {

			BufferCopyRegion region = {};
			region.Offset = 0;
			region.Size = readbackBuffer->GetSize();

			Buffer data = readbackBuffer->ReadData(region);
			info.Job->SetResult(data);
			info.ParentGraph->Continue();
		});

		info.ParentGraph->Halt();
	}

	void ImageAssetLoader::GenerateImageBinary(JobInfo& info, Ref<HazardRenderer::Image2D> image)
	{
		Buffer imageData = info.ParentGraph->GetResult<Buffer>();

		ImageAssetFileHeader file = {};
		file.Extent = image->GetExtent();
		file.Format = image->GetFormat();

		Buffer assetData;
		assetData.Allocate(sizeof(ImageAssetFileHeader) + imageData.Size);

		CachedBuffer buf(assetData.Data, assetData.Size);
		buf.Write(file);
		buf.Write(imageData);

		info.Job->SetResult(assetData),
			imageData.Release();
	}

	void ImageAssetLoader::CreateImageFromBinary(JobInfo& info, AssetHandle handle)
	{
		AssetMetadata& metadata = AssetManager::GetMetadata(handle);

		if (!File::Exists(metadata.FilePath))
			throw JobException("File does not exist");

		CachedBuffer buffer = File::ReadBinaryFile(metadata.FilePath);
		AssetPack pack = {};
		pack.FromBuffer(buffer);

		CachedBuffer data(pack.AssetData.Data, pack.AssetData.Size);
		ImageAssetFileHeader header = data.Read<ImageAssetFileHeader>();

		Ref<Texture2DAsset> asset = Ref<Texture2DAsset>::Create();
		asset->SetExtent(header.Extent);
		asset->SetMaxMipLevels(1);
		asset->Invalidate(data.Read<Buffer>(data.GetSize() - data.GetCursor()));

		info.Job->SetResult(asset);
	}
}
