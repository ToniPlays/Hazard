#include <hzrpch.h>
#include "EnvironmentAssetLoader.h"

#include "Hazard/Assets/AssetManager.h"
#include "EnvironmentMap.h"

namespace Hazard
{
	static void CreateEnvironmentAssetJob(Ref<Job> job, Ref<EnvironmentMap> asset, const std::filesystem::path& path)
	{
		EnvironmentAssetHeader header = {};
		header.ImageHandle = 0;
		header.Samples = 128;
		header.Resolution = 4096;

		const AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());
		const AssetMetadata& packMetadata = AssetManager::GetMetadata(metadata.AssetPackHandle);

		std::filesystem::path filePath = packMetadata.Handle != INVALID_ASSET_HANDLE ? packMetadata.Key : path;
		AssetHandle packHandle = metadata.AssetPackHandle != INVALID_ASSET_HANDLE ? metadata.AssetPackHandle : AssetHandle();

		AssetPackElement element = {};
		element.AssetPackHandle = packHandle;
		element.Handle = asset->GetHandle();
		element.Type = AssetType::EnvironmentMap;
		element.Data = Buffer(&header, sizeof(EnvironmentAssetHeader));
		element.AddressableName = File::GetName(path);

		AssetPack pack = {};
		pack.Handle = packHandle;
		pack.ElementCount = 1;
		pack.Elements = { element };

		CachedBuffer packBuffer = AssetPack::ToBuffer(pack);

		if (!File::WriteBinaryFile(filePath, packBuffer.GetData(), packBuffer.GetSize()))
			throw JobException(fmt::format("Saving asset pack failed: {0}", filePath.string()));
	}

	static void LoadEnvironmentMapJob(Ref<Job> job, AssetHandle handle)
	{
		Buffer assetData = AssetManager::GetAssetData(handle);
		EnvironmentAssetHeader header = assetData.Read<EnvironmentAssetHeader>();
		header.ImageHandle = 13937746866108958146;
		assetData.Release();

		Ref<Texture2DAsset> sourceTexture = AssetManager::GetAsset<Texture2DAsset>(header.ImageHandle);
		if (!sourceTexture)
			throw JobException(fmt::format("Loading environment map failed: Cannot find source image"));

		Ref<EnvironmentMap> map = Ref<EnvironmentMap>::Create();
		map->Update(header.Samples, header.Resolution, sourceTexture);

		map->IncRefCount();
		job->GetStage()->SetResult(map);
	}

	Ref<JobGraph> EnvironmentAssetLoader::Load(AssetMetadata& metadata)
	{
		Ref<Job> job = Ref<Job>::Create(LoadEnvironmentMapJob, metadata.Handle);

		Ref<JobGraph> graph = Ref<JobGraph>::Create(fmt::format("EnvironmentMapLoad load {}", metadata.Handle), 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}
	Ref<JobGraph> EnvironmentAssetLoader::Save(Ref<Asset>& asset)
	{
		return Ref<JobGraph>();
	}
	Ref<JobGraph> EnvironmentAssetLoader::Create(const std::filesystem::path& path)
	{
		Ref<EnvironmentMap> environmentMap = Ref<EnvironmentMap>::Create();
		environmentMap->m_Handle = AssetHandle();

		Ref<Job> createJob = Ref<Job>::Create(CreateEnvironmentAssetJob, environmentMap, path);

		Ref<JobGraph> graph = Ref<JobGraph>::Create("EnvironmentAsset Create", 1);
		graph->GetStage(0)->QueueJobs({ createJob });

		return graph;
	}
}
