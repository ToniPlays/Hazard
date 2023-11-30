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
        Ref<JobGraph> Save(Ref<Asset>& asset) override;
        Ref<JobGraph> DataFromSource(const std::filesystem::path& path) override { return nullptr; };
        Ref<JobGraph> Create(const std::filesystem::path& path, const std::filesystem::path& internalPath) override;

        Buffer ToBinary(Ref<Asset> asset) override { return Buffer(); };
    };
}
