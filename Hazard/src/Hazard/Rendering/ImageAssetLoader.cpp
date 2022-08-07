
#include <hzrpch.h>
#include "ImageAssetLoader.h"
#include "TextureFactory.h"
#include "Texture2D.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard 
{
	bool ImageAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		using namespace HazardRenderer;
		TextureHeader header = TextureFactory::LoadTexture(metadata.Path.string());
		
		Image2DCreateInfo info = {};
		info.Width = header.Width;
		info.Height = header.Height;
		info.Data = header.ImageData;
		info.Format = info.Format;
		info.ClearLocalBuffer = true;

		Ref<Hazard::Image2D> imageAsset = HazardRenderer::Image2D::Create(&info);
		imageAsset->m_Flags = AssetFlags::RuntimeGenerated;

		AssetMetadata medatata = {};
		metadata.Type = AssetType::Image;
		AssetManager::AddRuntimeAsset(metadata, imageAsset);

		asset = Ref<Texture2D>::Create(imageAsset);

		return asset;
	}
	bool ImageAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}