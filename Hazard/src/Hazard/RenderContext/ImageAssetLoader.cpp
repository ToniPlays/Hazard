
#include <hzrpch.h>
#include "ImageAssetLoader.h"
#include "TextureFactory.h"
#include "Texture2D.h"
#include "Hazard/Assets/AssetManager.h"

#include "Hazard/Assets/AssetPack.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{
	LoadType ImageAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();

		JobPromise promise = LoadAsync(metadata);
		promise.Wait();

		return (LoadType)promise.ReturnCode();
	}
	JobPromise ImageAssetLoader::LoadAsync(AssetMetadata& metadata)
	{
		using namespace HazardRenderer;
		return Application::Get().SubmitJob<Ref<Texture2DAsset>>("Image", [path = metadata.Path, handle = metadata.Handle](JobSystem* system, Job* job) -> size_t {

			if (TextureFactory::CacheStatus(handle) == CacheStatus::Exists)
			{
				auto cachedPath = TextureFactory::GetCacheFile(handle);
				CachedBuffer buffer = File::ReadBinaryFile(cachedPath);
				AssetPackElement element = buffer.Read<AssetPackElement>();

				TextureAssetHeader header = buffer.Read<TextureAssetHeader>();
				Buffer imageData = buffer.Read<Buffer>(header.Channels * header.Width * header.Height);

				Image2DCreateInfo info = {};
				info.DebugName = File::GetName(path);
				info.Width = header.Width;
				info.Height = header.Height;
				info.Data = imageData;
				info.Format = ImageFormat::RGBA;
				info.Usage = ImageUsage::Texture;
				info.ClearLocalBuffer = true;
				info.GenerateMips = false;

				Ref<Image2D> image = Image2D::Create(&info);
				Ref<AssetPointer> pointer = AssetPointer::Create(image, AssetType::Image);

				Ref<Texture2DAsset> asset = Ref<Texture2DAsset>::Create(pointer);
				asset->m_Type = AssetType::Image;

				*job->Value<Ref<Texture2DAsset>>() = std::move(asset);

				return (size_t)LoadType::Cache;
			}

			TextureHeader header = TextureFactory::LoadTextureFromSourceFile(path, true);

			Image2DCreateInfo info = {};
			info.DebugName = File::GetName(path);
			info.Width = header.Width;
			info.Height = header.Height;
			info.Data = header.ImageData;
			info.Format = ImageFormat::RGBA;
			info.Usage = ImageUsage::Texture;
			info.ClearLocalBuffer = true;
			info.GenerateMips = true;

			Ref<Image2D> image = Image2D::Create(&info);
			Ref<AssetPointer> pointer = AssetPointer::Create(image, AssetType::Image);

			Ref<Texture2DAsset> asset = Ref<Texture2DAsset>::Create(pointer);
			asset->m_Type = AssetType::Image;

			header.ImageData.Release();

			*job->Value<Ref<Texture2DAsset>>() = std::move(asset);
			return (size_t)LoadType::Source;
			});
	}
	bool ImageAssetLoader::Save(Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();

		return false;
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

		return false;
	}
	JobPromise ImageAssetLoader::SaveAsync(Ref<Asset>& asset)
	{
		HZR_CORE_ASSERT(false, "TODO");
		return JobPromise();
	}
}