
#include <hzrpch.h>
#include "EnvironmentAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "HazardRendererCore.h"
#include "EnvironmentMap.h"

namespace Hazard 
{
	bool EnvironmentAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		Timer timer;
		using namespace HazardRenderer;

		HZR_CORE_INFO("Somehow generate the cubemap for environment map");

		CubemapTextureCreateInfo radianceInfo = {};
		radianceInfo.FilePath = metadata.Path.string();
		radianceInfo.Usage = ImageUsage::Texture;
		radianceInfo.Width = 2048;
		radianceInfo.Height = 2048;

		Ref<CubemapTexture> radianceMap = CubemapTexture::Create(&radianceInfo);
		if (radianceMap) 
		{
			AssetMetadata radianceMetadata = {};
			radianceMetadata.Handle = radianceMap->GetHandle();
			radianceMetadata.Type = AssetType::EnvironmentMap;

			AssetManager::AddRuntimeAsset(radianceMetadata, radianceMap);

			CubemapTextureCreateInfo irradianceInfo = {};
			irradianceInfo.Usage = ImageUsage::Texture;
			irradianceInfo.pCubemap = radianceMap;

			Ref<CubemapTexture> irradianceMap = CubemapTexture::Create(&irradianceInfo);

			AssetMetadata irradianceMetadata = {};
			irradianceMetadata.Handle = radianceMap->GetHandle();
			irradianceMetadata.Type = AssetType::EnvironmentMap;
			AssetManager::AddRuntimeAsset(irradianceMetadata, radianceMap);

			asset = Ref<EnvironmentMap>::Create();
			asset.As<EnvironmentMap>()->RadianceMap = radianceMap;
			asset.As<EnvironmentMap>()->IrradianceMap = irradianceMap;
		}

		HZR_CORE_WARN("Environment map took {0} ms to load", timer.ElapsedMillis());

		return asset;
	}
	bool EnvironmentAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}