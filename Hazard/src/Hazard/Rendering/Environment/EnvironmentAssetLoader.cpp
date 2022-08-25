
#include <hzrpch.h>
#include "EnvironmentAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "HazardRendererCore.h"
#include "EnvironmentMap.h"

namespace Hazard 
{
	bool EnvironmentAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		using namespace HazardRenderer;

		HZR_CORE_INFO("Somehow generate the cubemap for environment map");

		CubemapTextureCreateInfo radianceInfo = {};
		radianceInfo.FilePath = metadata.Path.string();
		radianceInfo.Usage = ImageUsage::Texture;
		radianceInfo.Width = 1024;
		radianceInfo.Height = 1024;

		Ref<CubemapTexture> radianceMap = CubemapTexture::Create(&radianceInfo);
		AssetMetadata radianceMetadata = {};
		radianceMetadata.Handle = radianceMap->GetHandle();
		radianceMetadata.Type = AssetType::EnvironmentMap;
		AssetManager::AddRuntimeAsset(radianceMetadata, radianceMap);

		asset = Ref<EnvironmentMap>::Create();
		asset.As<EnvironmentMap>()->RadianceMap = radianceMap;

		return asset;
	}
	bool EnvironmentAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}