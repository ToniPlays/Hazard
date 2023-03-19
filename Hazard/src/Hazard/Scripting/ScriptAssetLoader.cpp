
#include <hzrpch.h>
#include "ScriptAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "HScript.h"

namespace Hazard 
{
	LoadType ScriptAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags)
	{
		HZR_PROFILE_FUNCTION();
		asset = Ref<HScript>::Create(File::GetNameNoExt(metadata.Path));
		return LoadType::Source;
	}
	/*Ref<JobGraph> ScriptAssetLoader::LoadAsync(AssetMetadata& metadata, uint32_t flags)
	{
		return nullptr;
	}*/
	bool ScriptAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
	/*Ref<JobGraph> ScriptAssetLoader::SaveAsync(Ref<Asset>& asset)
	{
		return nullptr;
	}*/
}