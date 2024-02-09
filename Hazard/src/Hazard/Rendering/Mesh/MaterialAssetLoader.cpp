#include <hzrpch.h>
#include "MaterialAssetLoader.h"

#include "Hazard/Assets/AssetManager.h"
#include "Material.h"

namespace Hazard
{
    Ref<JobGraph> MaterialAssetLoader::Load(AssetMetadata& metadata)
    {
        return nullptr;
    }

    Ref<JobGraph> MaterialAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
    {
        return nullptr;
    }

    Ref<JobGraph> MaterialAssetLoader::Create(const CreateAssetSettings& settings)
    {
        return nullptr;
    }
}

