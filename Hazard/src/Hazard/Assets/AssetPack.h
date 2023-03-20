#pragma once

#include "Asset.h"

namespace Hazard
{
	struct AssetPack
	{
		uint32_t Elements;
		CachedBuffer ElementBuffer;
	};

	struct AssetPackElement
	{
		uint64_t Handle;
		uint32_t Type;
		size_t AssetDataSize = 0;
	};
}