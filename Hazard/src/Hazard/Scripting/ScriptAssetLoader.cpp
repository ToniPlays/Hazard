
#include <hzrpch.h>
#include "ScriptAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "HScript.h"

namespace Hazard 
{
	Ref<JobGraph> ScriptAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		asset = Ref<HScript>::Create(File::GetNameNoExt(metadata.Path));
		return nullptr;
	}
	Ref<JobGraph> ScriptAssetLoader::Save(Ref<Asset>& asset)
	{
		return nullptr;
	}
}