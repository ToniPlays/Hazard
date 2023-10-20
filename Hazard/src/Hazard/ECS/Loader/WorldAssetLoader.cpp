
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

		std::string filePath = packMetadata.Handle != INVALID_ASSET_HANDLE ? packMetadata.Key : path.string();
        
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
		buffer.Release();

		if (!File::WriteBinaryFile(filePath, packBuffer.GetData(), packBuffer.GetSize()))
			throw JobException(fmt::format("Saving asset pack failed: {0}", filePath));
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
		job->SetResult(&world, sizeof(Ref<World>));
	}

	Ref<JobGraph> WorldAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();

		WorldDeserializer deserializer(metadata.Handle);

		Ref<Job> preprocessJob = Ref<Job>::Create("Preprocessing world", WorldPreprocessJob, deserializer);

		Ref<JobGraph> graph = Ref<JobGraph>::Create("World load", 1);
		graph->GetStage(0)->QueueJobs({ preprocessJob });
		graph->GetStage(0)->SetWeight(1.0f);

		return graph;
	}
	Ref<JobGraph> WorldAssetLoader::Save(Ref<Asset>& asset)
	{
		AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());
		auto world = asset.As<World>();

		Ref<Job> job = Ref<Job>::Create("Save world", SaveWorld, world, metadata.Key);
		Ref<JobGraph> graph = Ref<JobGraph>::Create("World save", 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}
	Ref<JobGraph> WorldAssetLoader::Create(const std::filesystem::path& path)
	{
		AssetHandle handle = AssetHandle();
		Ref<World> world = Ref<World>::Create(std::to_string(handle));
		world->m_Handle = handle;

		Ref<Job> job = Ref<Job>::Create("Save world", SaveWorld, world, path);
		Ref<JobGraph> graph = Ref<JobGraph>::Create("World save", 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}
}
