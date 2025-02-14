
#include <hzrpch.h>
#include "Asset.h"
#include "AssetManager.h"

namespace Hazard
{
	Asset::~Asset()
	{
		AssetManager::Unload(m_Handle);
	}

	void Asset::SetSourceFilePath(const std::filesystem::path& newPath)
	{
		m_SourceAssetPath = newPath; 
		AssetManager::GetMetadata(m_Handle).SourceFile = newPath;
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
