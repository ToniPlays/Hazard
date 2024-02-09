
#include <hzrpch.h>

#include "MeshAssetLoader.h"
#include "MeshFactory.h"
#include "Hazard/Assets/AssetManager.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{

	Ref<JobGraph> MeshAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();
		return nullptr;
	}

	Ref<JobGraph> MeshAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		return nullptr;
	}

	Ref<JobGraph> MeshAssetLoader::Create(const CreateAssetSettings& settings)
	{
		return nullptr;
	}
}
