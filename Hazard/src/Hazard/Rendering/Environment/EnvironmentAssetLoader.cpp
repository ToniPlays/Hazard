
#include <hzrpch.h>
#include "EnvironmentAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "HazardRendererCore.h"
#include "EnvironmentMap.h"
#include "Hazard/RenderContext/TextureFactory.h"

namespace Hazard 
{
	bool EnvironmentAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		return asset;
	}
	bool EnvironmentAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}