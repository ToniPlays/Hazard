
#include <hzrpch.h>
#include "WorldAssetLoader.h"
#include "WorldDeserializer.h"
#include "WorldSerializer.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{
	void SaveWorld(Ref<Job> job, Ref<World> world)
	{
		WorldSerializer serializer(world);
		serializer.SerializeEditor(world->GetWorldFile());
	}

	Ref<JobGraph> WorldAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();

		//If asset pack is empty, load from source file
		if (metadata.AssetPackFile.empty())
		{
			WorldDeserializer deserializer;
			return deserializer.DeserializeEditor(metadata.Key);
		}
		return nullptr;
	}
	Ref<JobGraph> WorldAssetLoader::Save(Ref<Asset>& asset)
	{
        auto world = asset.As<World>();
        
		Ref<Job> job = Ref<Job>::Create(SaveWorld, world);

		Ref<JobGraph> graph = Ref<JobGraph>::Create("World save", 1);
		graph->GetStage(0)->QueueJobs({ job });
        
        return graph;
	}
}
