#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard
{
    class Material;

    struct MaterialAssetHeader
    {
        AssetHandle PipelineHandle;
    };

    class MaterialAssetLoader : public IAssetLoader
    {
    public:
        struct CreateSettings
        {

        };

        MaterialAssetLoader() = default;
        ~MaterialAssetLoader() = default;
        
        Ref<JobGraph> Load(AssetMetadata& metadata) override;
        Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
        Ref<JobGraph> Create(const CreateAssetSettings& settings) override;

    private:
        static void LoadMaterialAsset(JobInfo& info, const std::filesystem::path& path);
        static void CreateMaterialAsset(JobInfo& info, const CreateSettings& settings);
        static void SaveMaterialAsset(JobInfo& info, Ref<Material> material);
    };
}
