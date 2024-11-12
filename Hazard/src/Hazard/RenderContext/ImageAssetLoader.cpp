
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
	Ref<JobGraph> ImageAssetLoader::Load(AssetMetadata& metadata, const LoadAssetSettings& settings)
	{
		HZR_PROFILE_FUNCTION();

		//Ref<Job> createJob = Job::Create(fmt::format("ImageLoad: {}", metadata.FilePath.string()), CreateImageFromBinary, metadata.Handle);

		JobGraphInfo info = {
			.Name = "Image load",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Create", 1.0f, { } } }
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> ImageAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		HZR_PROFILE_FUNCTION();
		HZR_CORE_ASSERT(settings.Flags & ASSET_MANAGER_COMBINE_ASSET, "Cannot override image source file");

		//Ref<Job> readbackJob = Job::Create("Readback", ReadImageDataFromGPU, asset.As<Texture2DAsset>()->GetSourceImage());
		//Ref<Job> processJob = Job::Create("Process", GenerateImageBinary, asset.As<Texture2DAsset>()->GetSourceImage());

		JobGraphInfo info = {
			.Name = "Image save",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Readback", 0.5f, {  } },
						{ "Process", 0.5f, {  } } }
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> ImageAssetLoader::Create(const CreateAssetSettings& settings)
	{
		auto& file = settings.SourcePath;

		CreateSettings imageSpec = {};
		if (settings.Settings)
			imageSpec = *(CreateSettings*)settings.Settings;

		Ref<Job> sourceLoad = Job::Create(fmt::format("Image data load from: {0}", file.string()), ImageDataLoadFromSource, file, imageSpec);
		Ref<Job> createImage = Job::Create(fmt::format("Image {}", File::GetName(file)), CreateImageFromData, imageSpec);

		JobGraphInfo info = {
			.Name = "Image load",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Load data", 0.8f, { sourceLoad } },
                { "Create", 0.2f, { createImage } }
			}
		};

		return Ref<JobGraph>::Create(info);
	}

    Coroutine ImageAssetLoader::ImageDataLoadFromSource(JobInfo& info, const std::filesystem::path& path, CreateSettings settings)
	{
		if (path.empty())
		{
			info.Result(TextureHeader());
			co_return;
		}

		if (!File::Exists(path))
			throw JobException(fmt::format("Image source file does not exist: {}", path.string()));

		TextureHeader header = TextureFactory::LoadTextureFromSourceFile(path, settings.FlipOnLoad);
		if (!header.ImageData.Data)
			throw JobException("Image load from source failed");

		info.Result(header);
	}

    Coroutine ImageAssetLoader::CreateImageFromData(JobInfo& info, CreateSettings settings)
	{
		TextureHeader header = info.Graph->GetResults<TextureHeader>()[0];

		Ref<Texture2DAsset> asset = Ref<Texture2DAsset>::Create();
		if (header.ImageData)
		{
			asset->SetExtent(header.Extent);
			asset->SetMaxMipLevels(header.Mips);
			asset->Invalidate(header.ImageData);
		}

		info.Result(asset);
		header.ImageData.Release();
        co_return;
	}

	Coroutine ImageAssetLoader::ReadImageDataFromGPU(JobInfo& info, Ref<HazardRenderer::Image2D> image)
	{
		using namespace HazardRenderer;

		ImageCopyRegion region = {
			.Extent = image->GetExtent(),
			.X = 0,
			.Y = 0,
			.Z = 0
		};

		BufferCreateInfo bufferInfo = {
			.Name = "Image readback",
			.UsageFlags = BUFFER_USAGE_STORAGE_BUFFER_BIT | BUFFER_USAGE_DYNAMIC,
			.Size = region.Extent.Width * region.Extent.Height * region.Extent.Depth * sizeof(float),
		};

		Ref<GPUBuffer> readbackBuffer = GPUBuffer::Create(&bufferInfo);

		ImageMemoryInfo barrier = {
			.Image = (Image*)image.Raw(),
			.BaseLayer = 0,
			.LayerCount = 1,
			.BaseMip = 0,
			.MipCount = 1,
			.SrcLayout = IMAGE_LAYOUT_SHADER_READ_ONLY,
			.DstLayout = IMAGE_LAYOUT_TRANSFER_SRC,
		};

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

			BufferCopyRegion region = {
				.Size = readbackBuffer->GetSize(),
				.Offset = 0
			};

			Buffer data = readbackBuffer->ReadData(region);
			info.Result(Ref<CachedBuffer>::Create(data));
		});
        co_return;
	}

	Coroutine ImageAssetLoader::GenerateImageBinary(JobInfo& info, Ref<HazardRenderer::Image2D> image)
	{
		Ref<CachedBuffer> imageData = info.Graph->GetResults<Ref<CachedBuffer>>()[0];

		ImageAssetFileHeader file = {
			.Extent = image->GetExtent(),
			.Format = image->GetFormat(),
		};

		Ref<CachedBuffer> buf = Ref<CachedBuffer>::Create();
		buf->Allocate(sizeof(ImageAssetFileHeader) + imageData->GetSize());
		buf->Write(file);
		buf->Write(imageData->GetData(), imageData->GetSize());

		info.Result(buf);
        co_return;
	}

	Coroutine ImageAssetLoader::CreateImageFromBinary(JobInfo& info, AssetHandle handle)
	{
		AssetMetadata& metadata = AssetManager::GetMetadata(handle);

		if (!File::Exists(metadata.FilePath))
			throw JobException("File does not exist");

		Ref<CachedBuffer> buffer = File::ReadBinaryFile(metadata.FilePath);
		AssetPack pack = {};
		pack.FromBuffer(buffer);

		ImageAssetFileHeader header = pack.AssetData->Read<ImageAssetFileHeader>();

		Ref<Texture2DAsset> asset = Ref<Texture2DAsset>::Create();
		asset->SetExtent(header.Extent);
		asset->SetMaxMipLevels(1);
		asset->Invalidate(pack.AssetData->Read<Buffer>(pack.AssetData->GetSize() - pack.AssetData->GetCursor()));

        info.Result(asset);
        co_return;
	}
}
