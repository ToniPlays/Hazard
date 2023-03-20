
#include <hzrpch.h>
#include "ImageAssetLoader.h"
#include "TextureFactory.h"
#include "Texture2D.h"
#include "Hazard/Assets/AssetManager.h"

#include "Hazard/Assets/AssetPack.h"
#include "Hazard/Core/Application.h"

namespace Hazard
{
	static void LoadImageJob(Ref<Job> job, AssetHandle handle)
	{
		using namespace HazardRenderer;

		Buffer buffer = AssetManager::GetAssetData(handle);
		CachedBuffer readBuffer(buffer.Data, buffer.Size);

		TextureFileHeader header = readBuffer.Read<TextureFileHeader>();

		Image2DCreateInfo info = {};
		info.DebugName = fmt::format("Image {}", handle);
		info.Extent.Width = header.Width;
		info.Extent.Height = header.Height;
		info.Data = readBuffer.Read<Buffer>(header.Width * header.Height * header.Channels);
		info.Format = ImageFormat::RGBA;
		info.Usage = ImageUsage::Texture;
		info.ClearLocalBuffer = true;
		info.GenerateMips = false;

		Ref<Image2D> image = Image2D::Create(&info);
		Ref<AssetPointer> pointer = AssetPointer::Create(image, AssetType::Image);

		Ref<Texture2DAsset> asset = Ref<Texture2DAsset>::Create(pointer);
		job->GetStage()->SetResult(asset);

		AssetManager::GetMetadata(handle).LoadState = LoadState::Loaded;
	}

	Ref<JobGraph> ImageAssetLoader::Load(AssetMetadata& metadata)
	{
		Ref<Job> job = Ref<Job>::Create(LoadImageJob, metadata.Handle);

		Ref<JobGraph> graph = Ref<JobGraph>::Create(fmt::format("Image load {}", metadata.Handle), 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}
	Ref<JobGraph> ImageAssetLoader::Save(Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();

		return nullptr;
		/*
		Thread& thread = Application::Get().GetThreadPool().GetThread();
		auto& metadata = AssetManager::GetMetadata(asset->GetHandle());

		AssetHandle handle = asset.As<Texture2DAsset>()->GetSourceImageAsset()->GetHandle();

		auto cachePath = TextureFactory::GetCacheFile(handle);

		thread.OnCompletionHandler([cachePath](const Thread& thisThread) {
			HZR_CORE_INFO("Image saved {0}, took {1} ms", cachePath.string(), thisThread.GetExecutionTime());
			});

		thread.Dispatch([assetHandle = handle, type = asset->GetType(), path = cachePath, sourcePath = metadata.Path]() {

			TextureHeader header = TextureFactory::LoadTextureFromSourceFile(sourcePath, true);
			CachedBuffer buffer(sizeof(AssetPackElement) + sizeof(TextureHeader) + header.ImageData.Size);

			AssetPackElement element = {};

			element.Type = (uint32_t)type;
			element.Handle = assetHandle;
			element.AssetDataSize = sizeof(TextureHeader) + header.ImageData.Size;

			TextureAssetHeader assetHeader = {};
			assetHeader.Channels = header.Channels;
			assetHeader.Dimensions = header.Dimensions;
			assetHeader.Width = header.Width;
			assetHeader.Height = header.Height;
			assetHeader.Format = (uint32_t)header.Format;

			buffer.Write(element);
			buffer.Write(assetHeader);
			buffer.Write(header.ImageData.Data, header.ImageData.Size);

			if (!File::DirectoryExists(path.parent_path()))
				File::CreateDir(path.parent_path());

			File::WriteBinaryFile(path, buffer.GetData(), buffer.GetSize());

		});
		*/
	}
}
