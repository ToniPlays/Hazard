#include <hzrpch.h>

#include "AssetLoader.h"
#include "AssetManager.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{
	Ref<JobGraph> AssetLoader::Load(AssetMetadata& metadata, const LoadAssetSettings& settings)
	{
		HZR_CORE_ASSERT(metadata.Type != AssetType::Undefined, "Asset type cannot be undefined");

		if (m_Loaders.find(metadata.Type) == m_Loaders.end())
		{
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.FilePath.string());
			return nullptr;
		}

		return m_Loaders[metadata.Type]->Load(metadata, settings);
	}

	Ref<JobGraph> AssetLoader::Save(Ref<Asset>& asset, const SaveAssetSettings& settings)
	{
		if (!asset)
			return nullptr;

		if (m_Loaders.find(asset->GetType()) == m_Loaders.end())
		{
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(asset->GetType()));
			return nullptr;
		}
		return m_Loaders[asset->GetType()]->Save(asset, settings);
	}

	Ref<JobGraph> AssetLoader::Create(AssetType type, const CreateAssetSettings& settings)
	{
		if (m_Loaders.find(type) == m_Loaders.end())
		{
			HZR_CORE_ERROR("No loaders for {0} ({1})", Utils::AssetTypeToString(type));
			return nullptr;
		}

		return m_Loaders[type]->Create(settings);
	}
}
