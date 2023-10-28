#include <hzrpch.h>

#include "AssetLoader.h"
#include "AssetManager.h"

#include "Hazard/Core/Application.h"

namespace Hazard 
{
	Ref<JobGraph> AssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_CORE_ASSERT(metadata.Type != AssetType::Undefined, "Asset type cannot be undefined");

		if (m_Loaders.find(metadata.Type) == m_Loaders.end()) 
		{
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.Key);
			return nullptr;
		}

		return m_Loaders[metadata.Type]->Load(metadata);
	}
	Ref<JobGraph> AssetLoader::Save(Ref<Asset>& asset)
	{
		if (!asset) 
			return nullptr;

		AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());

		if (m_Loaders.find(metadata.Type) == m_Loaders.end()) 
		{
			HZR_CORE_ERROR("No loaders for {0} : {1}", Utils::AssetTypeToString(metadata.Type), metadata.Key);
			return nullptr;
		}
		return m_Loaders[metadata.Type]->Save(asset);
	}

	Ref<JobGraph> AssetLoader::FromSourceFile(const std::filesystem::path& path)
	{
		if (!File::Exists(path))
			return nullptr;

		AssetType type = Utils::AssetTypeFromExtension(File::GetFileExtension(path));

		if (m_Loaders.find(type) == m_Loaders.end())
		{
			HZR_CORE_ERROR("No loaders for {0}", Utils::AssetTypeToString(type));
			return nullptr;
		}

		return m_Loaders[type]->FromSourceFile(path);
	}

	Ref<JobGraph> AssetLoader::Create(AssetType type, const std::filesystem::path& path)
	{
		if (m_Loaders.find(type) == m_Loaders.end())
		{
			HZR_CORE_ERROR("No loaders for {0}", Utils::AssetTypeToString(type));
			return nullptr;
		}

		return m_Loaders[type]->Create(path);
	}
	Buffer AssetLoader::AssetToBinary(Ref<Asset> asset)
	{
		AssetType type = asset->GetType();

		if (m_Loaders.find(type) == m_Loaders.end())
		{
			HZR_CORE_ERROR("No loaders for {0}", Utils::AssetTypeToString(type));
			return Buffer();
		}

		return m_Loaders[type]->ToBinary(asset);
	}
}
