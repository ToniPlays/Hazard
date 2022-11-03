#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard 
{
	struct TextureAssetHeader
	{
		uint32_t Width, Height;
		uint32_t Dimensions;
		uint32_t Channels;
		uint32_t Format;
	};


	class ImageAssetLoader : public IAssetLoader 
	{
	public:
		ImageAssetLoader() = default;
		~ImageAssetLoader() = default;

		LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset);
		bool Save(Ref<Asset>& asset);
	};
}