#pragma once

#include <hzrpch.h>
#include "AssetLoader.h"
#include "AssetManager.h"

namespace Hazard 
{
	bool AssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		if (m_Loaders.find(metadata.Type) == m_Loaders.end()) {
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.Path.string());
			return false;
		}
		bool result = m_Loaders[metadata.Type]->Load(metadata, asset);
		return result;
	}
	bool AssetLoader::Save(Ref<Asset>& asset)
	{
		AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());
		if (!asset) 
			return false;

		if (m_Loaders.find(metadata.Type) == m_Loaders.end()) {
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.Path.string());
			return false;
		}
		return m_Loaders[metadata.Type]->Save(asset);
	}
}
