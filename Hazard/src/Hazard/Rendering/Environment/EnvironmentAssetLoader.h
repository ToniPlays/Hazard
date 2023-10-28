#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard
{
    struct EnvironmentAssetHeader
    {
        AssetHandle ImageHandle;
        uint32_t Resolution;
        uint32_t Samples;
    };

    class EnvironmentAssetLoader : public IAssetLoader
    {
    public:
        EnvironmentAssetLoader() = default;
        ~EnvironmentAssetLoader() = default;

        Ref<JobGraph> Load(AssetMetadata& metadata) override;
        Ref<JobGraph> Save(Ref<Asset>& asset) override;
        Ref<JobGraph> FromSourceFile(const std::filesystem::path& path) override { return nullptr;  };
        Ref<JobGraph> Create(const std::filesystem::path& path) override;

        Buffer ToBinary(Ref<Asset> asset) override { return Buffer(); };
    };
}
#pragma once
