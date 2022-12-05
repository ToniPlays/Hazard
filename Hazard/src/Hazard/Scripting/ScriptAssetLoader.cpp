
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
	TypedJobPromise<Ref<Asset>> ScriptAssetLoader::LoadAsync(AssetMetadata& metadata, uint32_t flags)
	{
		return TypedJobPromise<Ref<Asset>>();
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