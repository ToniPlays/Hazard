
#include <hzrpch.h>
#include "WorldAssetLoader.h"
#include "WorldDeserializer.h"
#include "WorldSerializer.h"

namespace Hazard
{
	LoadType WorldAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		if (!File::Exists(metadata.Path))
			return LoadType::Failed;

		WorldDeserializer deserializer;
		asset = deserializer.DeserializeEditor(metadata.Path);

		return LoadType::Source;
	}
	bool WorldAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}