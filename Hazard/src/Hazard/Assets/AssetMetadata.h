#pragma once

#include "Asset.h"
#include "AssetEnums.h"

namespace Hazard 
{
	struct AssetMetadata 
	{
		AssetHandle Handle;
		AssetType Type = AssetType::Undefined;
		
		std::filesystem::path Path = "";
		bool IsLoaded = false;

		bool IsValid() { return Handle && Type != AssetType::Undefined && IsLoaded; }
	};
}