#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard
{
    struct MaterialAssetHeader
    {
        AssetHandle PipelineHandle;
        uint32_t TextureCount;
        uint32_t ParameterCount;
    };

    class MaterialAssetLoader : public IAssetLoader
    {
    public:
        MaterialAssetLoader() = default;
        ~MaterialAssetLoader() = default;
        
        Ref<JobGraph> Load(AssetMetadata& metadata) override;
        Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
        Ref<JobGraph> Create(const CreateAssetSettings& settings) override;
    };
}
