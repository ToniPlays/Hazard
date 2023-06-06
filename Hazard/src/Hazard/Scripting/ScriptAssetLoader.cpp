
#include <hzrpch.h>
#include "ScriptAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "HScript.h"

namespace Hazard 
{
	Ref<JobGraph> ScriptAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();
		return nullptr;
	}
	Ref<JobGraph> ScriptAssetLoader::Save(Ref<Asset>& asset)
	{
		return nullptr;
	}
}