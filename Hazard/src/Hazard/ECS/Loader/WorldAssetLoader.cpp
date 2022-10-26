
#include <hzrpch.h>
#include "WorldAssetLoader.h"
#include "WorldDeserializer.h"
#include "WorldSerializer.h"

namespace Hazard
{
	bool WorldAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		WorldDeserializer deserializer;
		Ref<World> world = deserializer.DeserializeEditor(metadata.Path);
		asset = world.As<Asset>();
		return asset;
	}
	bool WorldAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}