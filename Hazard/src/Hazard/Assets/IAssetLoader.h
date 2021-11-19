#pragma once

#include "Asset.h"

namespace Hazard 
{
	class IAssetLoader {
	public:
		IAssetLoader() = default;
		~IAssetLoader() = default;

		virtual bool Load(AssetMetadata& path, Ref<Asset>& asset) = 0;
		//TODO: Maybe a Revert option
		virtual bool Save(Ref<Asset>& asset) = 0;
	};
}