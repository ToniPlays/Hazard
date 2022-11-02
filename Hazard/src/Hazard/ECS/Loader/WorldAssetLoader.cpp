
#include <hzrpch.h>
#include "WorldAssetLoader.h"
#include "WorldDeserializer.h"
#include "WorldSerializer.h"

namespace Hazard
{
	LoadType WorldAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		if (!File::Exists(metadata.Path)) return LoadType::Failed;

		WorldDeserializer deserializer;
		Ref<World> world = deserializer.DeserializeEditor(metadata.Path);
		asset = world.As<Asset>();

		return LoadType::Source;
	}
	bool WorldAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}