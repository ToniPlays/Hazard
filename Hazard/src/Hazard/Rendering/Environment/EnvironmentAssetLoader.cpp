#include <hzrpch.h>
#include "EnvironmentAssetLoader.h"

#include "Hazard/Assets/AssetManager.h"
#include "EnvironmentMap.h"

namespace Hazard
{
	Ref<JobGraph> EnvironmentAssetLoader::Load(AssetMetadata& metadata)
	{
		return nullptr;
	}

	Ref<JobGraph> EnvironmentAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		return nullptr;
	}

	Ref<JobGraph> EnvironmentAssetLoader::Create(const CreateAssetSettings& settings)
	{
		return nullptr;
	}
}
