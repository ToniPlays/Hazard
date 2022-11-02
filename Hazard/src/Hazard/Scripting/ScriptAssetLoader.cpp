
#include <hzrpch.h>
#include "ScriptAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "HScript.h"

namespace Hazard 
{
	LoadType ScriptAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		asset = Ref<HScript>::Create(File::GetNameNoExt(metadata.Path));
		return LoadType::Source;
	}
	bool ScriptAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}