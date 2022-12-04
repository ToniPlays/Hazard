
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
	JobPromise ScriptAssetLoader::LoadAsync(AssetMetadata& metadata)
	{
		return JobPromise();
	}
	bool ScriptAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
	JobPromise ScriptAssetLoader::SaveAsync(Ref<Asset>& asset)
	{
		return JobPromise();
	}
}