
#include <hzrpch.h>
#include "ImageAssetLoader.h"
#include "TextureFactory.h"
#include "Texture2D.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	bool ImageAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		TextureHeader header = TextureFactory::GetFromCacheOrReload(metadata.Handle, metadata.Path);

		Image2DCreateInfo info = {};
		info.DebugName = File::GetName(metadata.Path);
		info.Width = header.Width;
		info.Height = header.Height;
		info.Data = header.ImageData;
		info.Format = ImageFormat::RGBA;
		info.Usage = ImageUsage::Texture;
		info.ClearLocalBuffer = true;

		Ref<Image2D> image = Image2D::Create(&info);
		Ref<AssetPointer> pointer = AssetPointer::Create(image, AssetType::Image);

		asset = Ref<Texture2DAsset>::Create(pointer);
		asset->m_Type = AssetType::Image;

		return asset;
	}
	bool ImageAssetLoader::Save(Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		return false;
	}
}