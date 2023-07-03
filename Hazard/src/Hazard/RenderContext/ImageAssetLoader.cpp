
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
	static void LoadImageJob(Ref<Job> job, AssetHandle handle)
	{
		using namespace HazardRenderer;

		Buffer buffer = AssetManager::GetAssetData(handle);
		if (!buffer.Data)
		{
			HZR_CORE_ERROR("Cannot find asset data for {}", handle);
			return; 
		}

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
		Ref<Sampler> sampler = RenderEngine::GetResources().DefaultImageSampler;

		Ref<Texture2DAsset> asset = Ref<Texture2DAsset>::Create(pointer, sampler);
		asset->IncRefCount();
		job->GetStage()->SetResult(asset);

		AssetManager::GetMetadata(handle).LoadState = LoadState::Loaded;

		buffer.Release();
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
	}
	Ref<JobGraph> ImageAssetLoader::Create(const std::filesystem::path& path)
	{
		HZR_ASSERT(false, "");
		return Ref<JobGraph>();
	}
}
