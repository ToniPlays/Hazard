
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
		info.DebugName = File::GetName(metadata.Path);
		info.Width = header.Width;
		info.Height = header.Height;
		info.Data = header.ImageData;
		info.Format = ImageFormat::RGBA;
		info.Usage = ImageUsage::Texture;
		info.ClearLocalBuffer = true;

		Ref<Hazard::Image2DAsset> imageAsset = Ref<Hazard::Image2DAsset>::Create(&info);
		imageAsset->m_Flags = AssetFlags::RuntimeGenerated;

		AssetMetadata imageData = {};
		imageData.Type = AssetType::Image;
		imageData.Handle = imageAsset->GetHandle();

		AssetManager::AddRuntimeAsset(imageData, imageAsset);
		asset = Ref<Texture2DAsset>::Create(imageAsset);
		asset->m_Type = AssetType::Image;


		return asset;
	}
	bool ImageAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}