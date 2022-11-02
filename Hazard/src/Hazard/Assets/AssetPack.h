#pragma once

#include "Asset.h"

namespace Hazard
{
	struct AssetPack
	{

	};

	struct AssetPackElement
	{
		UID Handle;
		AssetType Type;
		size_t AssetDataSize = 0;
	};
}