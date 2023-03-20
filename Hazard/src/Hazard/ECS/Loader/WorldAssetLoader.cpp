
#include <hzrpch.h>
#include "WorldAssetLoader.h"
#include "WorldDeserializer.h"
#include "WorldSerializer.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{
	Ref<JobGraph> WorldAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();

		WorldDeserializer deserializer;
		//asset = deserializer.DeserializeEditor(metadata);

		return nullptr;
	}
	Ref<JobGraph> WorldAssetLoader::Save(Ref<Asset>& asset)
	{
        auto world = asset.As<World>();
        
        WorldSerializer serializer(world);
        serializer.SerializeEditor(world->GetWorldFile());
        
        return nullptr;
	}
}
