#include <hzrpch.h>
#include "MaterialAssetLoader.h"

#include "Hazard/Assets/AssetManager.h"

#include "Backend/Core/Utils.h"

#include "Material.h"

namespace Hazard
{
    Ref<JobGraph> MaterialAssetLoader::Load(AssetMetadata& metadata)
    {
        return nullptr;
    }
    Ref<JobGraph> MaterialAssetLoader::Save(Ref<Asset>& asset)
    {
        return nullptr;
    }
    Ref<JobGraph> MaterialAssetLoader::Create(const std::filesystem::path& path)
    {
        return nullptr;
    }
}

