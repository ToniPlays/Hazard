#pragma once

#include "Hazard/ImGUI/Modal.h"
#include "Hazard/Assets/Asset.h"
#include "Hazard/Rendering/Mesh/MeshCreateInfo.h"

namespace UI
{
    struct ImageImportSettings
    {
        HazardRenderer::FilterMode MinFilter;
        HazardRenderer::FilterMode MagFilter;
        HazardRenderer::ImageWrap Wrapping;
        bool GenerateMips;
        bool FlipOnLoad;
        AssetHandle Handle;
    };
    struct MeshImportSettings
    {
        float Scale = 1.0;
    };

    class AssetImporterPanel : public Hazard::ImUI::Modal
    {
    public:
        AssetImporterPanel();
        ~AssetImporterPanel() = default;

        void Open(const std::filesystem::path& path);
        void OpenExisting(AssetHandle handle);

        void Update() override {};
        bool OnEvent(Event& e) override { return false; };
        void OnPanelRender() override;

    private:
        void DrawBottomBar();
        void DrawImageImportSettings();
        void DrawMeshImportSettings();

        void InitializeData();

        //Import jobs
        static void ImportImage(Ref<Job> job, std::filesystem::path filePath, std::filesystem::path destination, ImageImportSettings settings);
        static void ImportMesh(Ref<Job> job, std::filesystem::path filePath, std::filesystem::path destination, MeshImportSettings settings);

    private:
        std::filesystem::path m_CurrentFilePath;
        AssetType m_AssetType = AssetType::Undefined;

        Buffer m_ImportDataBuffer;
        AssetHandle m_AssetHandle;

        bool m_Import = false;
        bool m_IsImportingNew = false;
    };
}
