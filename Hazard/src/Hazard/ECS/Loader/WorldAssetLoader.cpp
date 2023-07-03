
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

		AssetHandle packHandle = AssetHandle();

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

		if (!File::WriteBinaryFile(path, packBuffer.GetData(), packBuffer.GetSize()))
		{
			buffer.Release();
			throw JobException(fmt::format("Saving asset pack failed: {0}", packHandle));
		}

		buffer.Release();
	}

	Ref<JobGraph> WorldAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();
		return nullptr;
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
		Ref<World> world = Ref<World>::Create(path);
		world->m_Handle = AssetHandle();

		Ref<Job> job = Ref<Job>::Create(SaveWorld, world, path);
		Ref<JobGraph> graph = Ref<JobGraph>::Create("World save", 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}
}
