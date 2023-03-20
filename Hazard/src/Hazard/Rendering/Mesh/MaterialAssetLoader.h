#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard
{
    class MaterialAssetLoader : public IAssetLoader
    {
    public:
        MaterialAssetLoader() = default;
        ~MaterialAssetLoader() = default;
        
        Ref<JobGraph> Load(AssetMetadata& metadata, Ref<Asset>& asset) override;
        //Ref<JobGraph> LoadAsync(AssetMetadata& metadata, uint32_t flags = 0) override;
        Ref<JobGraph> Save(Ref<Asset>& asset) override;
        //Ref<JobGraph> SaveAsync(Ref<Asset>& asset) override;
    };
}
