#include "MaterialAssetLoader.h"

#include "JobGraph.h"

namespace Hazard
{
    LoadType MaterialAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags)
    {
        return LoadType::Source;
    }
    Ref<JobGraph> MaterialAssetLoader::LoadAsync(AssetMetadata& metadata, uint32_t flags)
    {
        Ref<JobGraph> graph = Ref<JobGraph>::Create();
        return graph;
    }
    bool MaterialAssetLoader::Save(Ref<Asset>& asset)
    {
        return false;
    }
    Ref<JobGraph> MaterialAssetLoader::SaveAsync(Ref<Asset>& asset)
    {
        HZR_CORE_ASSERT(false, "TODO");
        return nullptr;
    }
}

