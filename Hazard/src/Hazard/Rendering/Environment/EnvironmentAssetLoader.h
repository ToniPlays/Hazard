#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard
{
    class EnvironmentMap;

    class EnvironmentAssetLoader : public IAssetLoader
    {
    public:
        EnvironmentAssetLoader() = default;
        ~EnvironmentAssetLoader() = default;

        Ref<JobGraph> Load(AssetMetadata& metadata) override;
        Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
        Ref<JobGraph> Create(const CreateAssetSettings& settings) override;

        struct CreateSettings
        {
            uint32_t Resolution = 2048;
            uint32_t Samples = 128;
        };

    private:
        static void SaveEnvironmentAsset(JobInfo& info, Ref<EnvironmentMap> map);
        static void CreateImageFromSource(JobInfo& info, const std::filesystem::path& sourcePath);
        static void GenerateEnvironmentMap(JobInfo& info, const CreateSettings& settings);
        static void CreateEnvironmentAsset(JobInfo& info, uint32_t samples);
    };
}
#pragma once
