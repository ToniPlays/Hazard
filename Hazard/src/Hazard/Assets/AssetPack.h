#pragma once

#include "Asset.h"

namespace Hazard
{
	struct AssetPack
	{

	};

	struct AssetPackElement
	{
		uint64_t Handle;
		uint32_t Type;
		size_t AssetDataSize = 0;
	};
}