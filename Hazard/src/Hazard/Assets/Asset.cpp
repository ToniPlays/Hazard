
#include <hzrpch.h>
#include "Asset.h"
#include "AssetManager.h"

namespace Hazard 
{
	Asset::~Asset()
	{
		AssetManager::RemoveAsset(m_Handle);
	}
}
