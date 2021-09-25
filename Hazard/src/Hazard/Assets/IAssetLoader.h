#pragma once

#include "Hazard/Core/Core.h"
#include "Asset.h"
#include "AssetMetadata.h"
#include <filesystem>

namespace Hazard 
{
	class IAssetLoader {
	public:
		IAssetLoader() = default;
		~IAssetLoader() = default;
		virtual bool Load(AssetMetadata& path, Ref<Asset>& asset) = 0;
		virtual bool Save(Ref<Asset>& asset) = 0;
	};
}