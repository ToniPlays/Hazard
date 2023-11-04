#include <hzrpch.h>
#include "EnvironmentAssetLoader.h"

#include "Hazard/Assets/AssetManager.h"
#include "EnvironmentMap.h"

namespace Hazard
{
	static void SaveEnvironmentMapJob(JobInfo& info, Ref<EnvironmentMap> asset, const std::filesystem::path& path)
	{
		EnvironmentAssetHeader header = {};
		header.ImageHandle = asset->GetSourceImageHandle();
		header.Samples = asset->GetSpec().Samples;
		header.Resolution = asset->GetSpec().Resolution;

		const AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());
		const AssetMetadata& packMetadata = AssetManager::GetMetadata(metadata.AssetPackHandle);

		std::string filePath = packMetadata.Handle != INVALID_ASSET_HANDLE ? packMetadata.Key : path.string();
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
			throw JobException(fmt::format("Saving asset pack failed: {0}", filePath));
	}

	static void LoadEnvironmentMapJob(JobInfo& info, AssetHandle handle)
	{
		Buffer assetData = AssetManager::GetAssetData(handle);
		EnvironmentAssetHeader header = assetData.Read<EnvironmentAssetHeader>();
		assetData.Release();

		Ref<EnvironmentMap> map = Ref<EnvironmentMap>::Create();
		map->Update(header.Samples, header.Resolution, header.ImageHandle);
		map->Invalidate();

		info.Job->SetResult(map);
	}

	Ref<JobGraph> EnvironmentAssetLoader::Load(AssetMetadata& metadata)
	{
		Ref<Job> job = Ref<Job>::Create(fmt::format("EnvironmentMapLoad load {}", metadata.Handle), LoadEnvironmentMapJob, metadata.Handle);

		Ref<JobGraph> graph = Ref<JobGraph>::Create(fmt::format("EnvironmentMapLoad load {}", metadata.Handle), 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}

	Ref<JobGraph> EnvironmentAssetLoader::Save(Ref<Asset>& asset)
	{
		const AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());

		Ref<Job> saveJob = Ref<Job>::Create("EnvironmentAsset Create", SaveEnvironmentMapJob, asset, metadata.Key);
		Ref<JobGraph> graph = Ref<JobGraph>::Create("EnvironmentAsset Create", 1);
		graph->GetStage(0)->QueueJobs({ saveJob });

		return graph;
	}

	Ref<JobGraph> EnvironmentAssetLoader::Create(const std::filesystem::path& path)
	{
		Ref<EnvironmentMap> environmentMap = Ref<EnvironmentMap>::Create();
		environmentMap->m_Handle = AssetHandle();

		Ref<Job> createJob = Ref<Job>::Create("EnvironmentAsset Create", SaveEnvironmentMapJob, environmentMap, path);
		Ref<JobGraph> graph = Ref<JobGraph>::Create("EnvironmentAsset Create", 1);
		graph->GetStage(0)->QueueJobs({ createJob });

		return graph;
	}
}
