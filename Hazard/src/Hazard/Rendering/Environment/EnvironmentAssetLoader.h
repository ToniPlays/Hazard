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
        Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
        Ref<JobGraph> Create(const CreateAssetSettings& settings) override;
    };
}
#pragma once
