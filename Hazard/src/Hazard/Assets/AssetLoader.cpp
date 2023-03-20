#include <hzrpch.h>

#include "AssetLoader.h"
#include "AssetManager.h"

#include "Hazard/Core/Application.h"

namespace Hazard 
{
	JobPromise<Ref<Asset>> AssetLoader::Load(AssetMetadata& metadata, bool synchronous)
	{
		HZR_CORE_ASSERT(metadata.Type != AssetType::Undefined, "Asset type cannot be undefined");

		if (m_Loaders.find(metadata.Type) == m_Loaders.end()) 
		{
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.Key);
			return JobPromise<Ref<Asset>>();
		}

		Ref<JobGraph> graph = m_Loaders[metadata.Type]->Load(metadata);
		if (synchronous)
		{
			graph->Execute();
			return JobPromise<Ref<Asset>>(graph);
		}

		return Application::Get().GetJobSystem().QueueGraph<Ref<Asset>>(graph);
	}
	JobPromise<bool> AssetLoader::Save(Ref<Asset>& asset)
	{
		if (!asset) 
			return JobPromise<bool>();

		AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());

		if (m_Loaders.find(metadata.Type) == m_Loaders.end()) 
		{
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.Key);
			return JobPromise<bool>();
		}
		return Application::Get().GetJobSystem().QueueGraph<bool>(m_Loaders[metadata.Type]->Save(asset));
	}
}
