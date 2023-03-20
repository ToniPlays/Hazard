
#include <hzrpch.h>
#include "WorldAssetLoader.h"
#include "WorldDeserializer.h"
#include "WorldSerializer.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{
	Ref<JobGraph> WorldAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		if (!File::Exists(metadata.Path))
			return nullptr;

		WorldDeserializer deserializer;
		asset = deserializer.DeserializeEditor(metadata.Path);

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
