
#include <hzrpch.h>
#include "ScriptAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "HScript.h"

namespace Hazard 
{
	bool ScriptAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		asset = Ref<HScript>::Create(File::GetNameNoExt(metadata.Path));
		return asset;
	}
	bool ScriptAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}