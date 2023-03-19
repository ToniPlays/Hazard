#include <hzrpch.h>

#include "AssetLoader.h"
#include "AssetManager.h"

#include "Hazard/Core/Application.h"

namespace Hazard 
{
	LoadType AssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags)
	{
		if (m_Loaders.find(metadata.Type) == m_Loaders.end()) 
		{
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.Path.string());
			return LoadType::Failed;
		}
		return m_Loaders[metadata.Type]->Load(metadata, asset);
	}
	/*Ref<JobGraph> AssetLoader::LoadAsync(AssetMetadata& metadata, uint32_t flags)
	{
		if (m_Loaders.find(metadata.Type) == m_Loaders.end())
		{
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.Path.string());
			return nullptr;
		}
		return m_Loaders[metadata.Type]->LoadAsync(metadata, flags);
	}*/
	bool AssetLoader::Save(Ref<Asset>& asset)
	{
		if (!asset) 
			return false;

		AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());

		if (m_Loaders.find(metadata.Type) == m_Loaders.end()) 
		{
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.Path.string());
			return false;
		}
		return m_Loaders[metadata.Type]->Save(asset);
	}
	/*Ref<JobGraph> AssetLoader::SaveAsync(Ref<Asset>& asset)
	{
		if (!asset)
			return nullptr;

		AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());

		if (m_Loaders.find(metadata.Type) == m_Loaders.end())
		{
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.Path.string());
			return nullptr;
		}
		return m_Loaders[metadata.Type]->SaveAsync(asset);
	}*/
}
