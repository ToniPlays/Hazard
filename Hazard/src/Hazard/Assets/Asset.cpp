#pragma once

#include <hzrpch.h>
#include "Asset.h"
#include "AssetManager.h"

namespace Hazard 
{
	AssetMetadata& Asset::GetMetadata()
	{
		return AssetManager::GetMetadata(m_Handle);
	}
}