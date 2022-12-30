#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard
{
    class MaterialAssetLoader : public IAssetLoader
    {
    public:
        MaterialAssetLoader() = default;
        ~MaterialAssetLoader() = default;
        
        LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags = 0) override {};
        Ref<JobGraph> LoadAsync(AssetMetadata& metadata, uint32_t flags = 0) override {};
        bool Save(Ref<Asset>& asset) override { return false; };
        Ref<JobGraph> SaveAsync(Ref<Asset>& asset) override {};
    };
}
