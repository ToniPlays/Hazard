#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard 
{
	class ImageAssetLoader : public IAssetLoader 
	{
	public:
		ImageAssetLoader() = default;
		~ImageAssetLoader() = default;

		LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset);
		bool Save(Ref<Asset>& asset);
	};
}