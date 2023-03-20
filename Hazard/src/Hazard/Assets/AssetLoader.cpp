#include <hzrpch.h>

#include "AssetLoader.h"
#include "AssetManager.h"

#include "Hazard/Core/Application.h"

namespace Hazard 
{
	JobPromise<bool> AssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags)
	{
		HZR_CORE_ASSERT(metadata.Type != AssetType::Undefined, "Asset type cannot be undefined");

		if (m_Loaders.find(metadata.Type) == m_Loaders.end()) 
		{
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.Path.string());
			return JobPromise<bool>();
		}
		return Application::Get().GetJobSystem().QueueGraph<bool>(m_Loaders[metadata.Type]->Load(metadata, asset));
	}
	JobPromise<bool> AssetLoader::Save(Ref<Asset>& asset)
	{
		if (!asset) 
			return JobPromise<bool>();

		AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());

		if (m_Loaders.find(metadata.Type) == m_Loaders.end()) 
		{
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.Path.string());
			return JobPromise<bool>();
		}
		return Application::Get().GetJobSystem().QueueGraph<bool>(m_Loaders[metadata.Type]->Save(asset));
	}
}
