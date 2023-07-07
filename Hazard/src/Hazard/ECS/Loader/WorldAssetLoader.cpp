
#include <hzrpch.h>
#include "WorldAssetLoader.h"
#include "WorldDeserializer.h"
#include "WorldSerializer.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{
	void SaveWorld(Ref<Job> job, Ref<World> world, std::filesystem::path path)
	{
		WorldSerializer serializer(world);
		Buffer buffer = serializer.Serialize();

		if (!buffer.Data)
			throw JobException(fmt::format("World serialization failed: {0}", path.string()));

		const AssetMetadata& metadata = AssetManager::GetMetadata(world->GetHandle());
		const AssetMetadata& packMetadata = AssetManager::GetMetadata(metadata.AssetPackHandle);

		std::filesystem::path filePath = packMetadata.Handle != INVALID_ASSET_HANDLE ? packMetadata.Key : path;
		AssetHandle packHandle = metadata.AssetPackHandle != INVALID_ASSET_HANDLE ? metadata.AssetPackHandle : AssetHandle();

		AssetPackElement element = {};
		element.AssetPackHandle = packHandle;
		element.Handle = world->GetHandle();
		element.Type = AssetType::World;
		element.Data = buffer;
		element.AddressableName = File::GetName(path);

		AssetPack pack = {};
		pack.Handle = packHandle;
		pack.ElementCount = 1;
		pack.Elements = { element };

		CachedBuffer packBuffer = AssetPack::ToBuffer(pack);

		if (!File::WriteBinaryFile(filePath, packBuffer.GetData(), packBuffer.GetSize()))
		{
			buffer.Release();
			throw JobException(fmt::format("Saving asset pack failed: {0}", filePath.string()));
		}

		buffer.Release();
	}

	static void WorldPreprocessJob(Ref<Job> job, WorldDeserializer deserializer)
	{
		Ref<World> world = deserializer.Deserialize();
		
		//Process all entities and get required assets to be loaded
		for (auto& entity : world->GetEntitiesWith<SpriteRendererComponent>())
		{
			Entity e = { entity, world.Raw() };
			auto& comp = e.GetComponent<SpriteRendererComponent>();
		}

		world->IncRefCount();
		job->GetStage()->SetResult(world);
	}

	Ref<JobGraph> WorldAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();

		WorldDeserializer deserializer(metadata.Handle);

		Ref<Job> preprocessJob = Ref<Job>::Create(WorldPreprocessJob, deserializer);
		preprocessJob->SetJobName("World preprocessor");

		Ref<JobGraph> graph = Ref<JobGraph>::Create("World load", 1);
		graph->GetStage(0)->QueueJobs({ preprocessJob });
		graph->GetStage(0)->SetWeight(1.0f);

		return graph;
	}
	Ref<JobGraph> WorldAssetLoader::Save(Ref<Asset>& asset)
	{
		AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());
		auto world = asset.As<World>();

		Ref<Job> job = Ref<Job>::Create(SaveWorld, world, metadata.Key);
		Ref<JobGraph> graph = Ref<JobGraph>::Create("World save", 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}
	Ref<JobGraph> WorldAssetLoader::Create(const std::filesystem::path& path)
	{
		Ref<World> world = Ref<World>::Create();
		world->m_Handle = AssetHandle();

		Ref<Job> job = Ref<Job>::Create(SaveWorld, world, path);
		Ref<JobGraph> graph = Ref<JobGraph>::Create("World save", 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}
}
