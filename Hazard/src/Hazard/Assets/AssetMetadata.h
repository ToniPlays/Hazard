#pragma once

#include "Asset.h"
#include "AssetEnums.h"

namespace Hazard 
{
	struct AssetMetadata 
	{
		AssetHandle Handle;
		AssetType Type;
		
		std::filesystem::path Path = "";
		bool IsLoaded = false;

		bool IsValid() { return Handle && Type != AssetType::None && IsLoaded; }
	};
}