
#include <hzrpch.h>
#include "Asset.h"
#include "AssetManager.h"

namespace Hazard
{
	Asset::~Asset()
	{
		AssetManager::Unload(m_Handle);
	}

	bool Asset::IsValid() const
	{
		return m_Handle != INVALID_ASSET_HANDLE && GetType() != AssetType::Undefined;
	}

	Ref<AssetPointer> AssetPointer::Create(Ref<RefCount> value, AssetType type)
	{
		Ref<AssetPointer> pointer = Ref<AssetPointer>::Create();
		pointer->Value = value;

		return pointer;
	}
}
