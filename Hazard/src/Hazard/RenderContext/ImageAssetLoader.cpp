
#include <hzrpch.h>
#include "ImageAssetLoader.h"
#include "TextureFactory.h"
#include "Texture2D.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Rendering/RenderEngine.h"

#include "Hazard/Assets/AssetPack.h"
#include "Hazard/Core/Application.h"

namespace Hazard
{
	Ref<JobGraph> ImageAssetLoader::Load(AssetMetadata& metadata)
	{
		Ref<Job> job = Ref<Job>::Create(fmt::format("Image load {}", metadata.Key), LoadImageJob, metadata.Handle);

		Ref<JobGraph> graph = Ref<JobGraph>::Create(fmt::format("Image load {}", metadata.Handle), 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}

	Ref<JobGraph> ImageAssetLoader::Save(Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		return nullptr;
	}

	Ref<JobGraph> ImageAssetLoader::DataFromSource(const std::filesystem::path& path)
	{
		Ref<Job> job = Ref<Job>::Create(fmt::format("Image From source {}", path.string()), LoadImageFromSourceJob, path);

		Ref<JobGraph> graph = Ref<JobGraph>::Create(fmt::format("Image create {}", path.string()), 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}

	Ref<JobGraph> ImageAssetLoader::Create(const std::filesystem::path& path)
	{
		HZR_ASSERT(false, "");
		return Ref<JobGraph>();
	}
	Buffer ImageAssetLoader::ToBinary(Ref<Asset> asset)
	{
		using namespace HazardRenderer;
		Ref<Image2D> image = asset.As<Texture2DAsset>()->GetSourceImageAsset()->Value;
		if (!image) return Buffer();

		ImageCopyRegion region = {};
		region.Extent = { image->GetWidth(), image->GetHeight(), 1 };
		region.X = 0;
		region.Y = 0;
		region.Z = 0;

		Buffer buffer = image->ReadPixels(region);

		TextureFileHeader header = {};
		header.Width = image->GetWidth();
		header.Height = image->GetHeight();
		header.Format = image->GetFormat();
		header.Channels = 4;
		header.Dimensions = 1;

		Buffer imageData;
		imageData.Allocate(sizeof(TextureFileHeader) + buffer.Size);
		imageData.Write(&header, sizeof(TextureFileHeader));
		imageData.Write(buffer.Data, buffer.Size, sizeof(TextureFileHeader));

		return imageData;
	}

	void ImageAssetLoader::LoadImageJob(JobInfo& info, AssetHandle handle)
	{
		using namespace HazardRenderer;

		AssetMetadata& metadata = AssetManager::GetMetadata(handle);
		Buffer buffer = AssetManager::GetAssetData(handle);

		if (!buffer.Data)
		{
			HZR_CORE_ERROR("Cannot find asset data for {}", handle);
			return;
		}

		CachedBuffer readBuffer(buffer.Data, buffer.Size);

		TextureFileHeader header = readBuffer.Read<TextureFileHeader>();

		Image2DCreateInfo imageInfo = {};
		imageInfo.DebugName = fmt::format("{0}", metadata.Key);
		imageInfo.Extent.Width = header.Width;
		imageInfo.Extent.Height = header.Height;
		imageInfo.Data = readBuffer.Read<Buffer>(header.Width * header.Height * header.Channels);
		imageInfo.Format = header.Format;
		imageInfo.Usage = ImageUsage::Texture;
		imageInfo.MaxMips = 32;

		Ref<Image2D> image = Image2D::Create(&imageInfo);
		Ref<AssetPointer> pointer = AssetPointer::Create(image, AssetType::Image);
		Ref<Sampler> sampler = RenderEngine::GetResources().DefaultImageSampler;

		Ref<Texture2DAsset> asset = Ref<Texture2DAsset>::Create(pointer, sampler);
		asset->m_Handle = handle;
		asset->m_Type = AssetType::Image;

		info.Job->SetResult(asset);
		buffer.Release();
	}

	void ImageAssetLoader::LoadImageFromSourceJob(JobInfo& info, std::filesystem::path& path)
	{
		TextureHeader header = TextureFactory::LoadTextureFromSourceFile(path, true);

		TextureFileHeader fileHeader = {};
		fileHeader.Width = header.Width;
		fileHeader.Height = header.Height;
		fileHeader.Channels = header.Channels;
		fileHeader.Dimensions = header.Dimensions;
		fileHeader.Format = header.Format;

		Buffer buffer;
		buffer.Allocate(sizeof(TextureFileHeader) + header.ImageData.Size);
		buffer.Write(&fileHeader, sizeof(TextureFileHeader));
		buffer.Write(header.ImageData.Data, header.ImageData.Size, sizeof(TextureFileHeader));

		AssetPackElement element = {};
		element.AddressableName = File::GetName(path);
		element.Type = AssetType::Image;
		element.AssetPackHandle = 0;
		element.Handle = UID();
		element.Data = buffer;

		info.Job->SetResult(element);
		header.ImageData.Release();
	}
}
