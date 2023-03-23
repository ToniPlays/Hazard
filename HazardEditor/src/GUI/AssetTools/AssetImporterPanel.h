#pragma once

#include "Hazard/ImGUI/Modal.h"
#include "Hazard/Assets/Asset.h"
#include "Hazard/Rendering/Mesh/MeshCreateInfo.h"
#include "Utility/YamlUtils.h"

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
        AssetHandle Handle;
    };

    class AssetImporterPanel : public Hazard::ImUI::Modal
    {
    public:
        AssetImporterPanel();
        ~AssetImporterPanel() = default;

        void Open(const std::filesystem::path& path);
        void OpenExisting(const std::filesystem::path& path, AssetHandle handle);

        void Update() override {};
        bool OnEvent(Event& e) override { return false; };
        void OnPanelRender() override;

    private:
        void DrawBottomBar();
        void DrawImageImportSettings();
        void DrawMeshImportSettings();

        void InitializeData();

        //Import jobs
        static void ImportImage(std::filesystem::path filePath, std::filesystem::path destination, ImageImportSettings settings);
        static void ImportMesh(std::filesystem::path filePath, std::filesystem::path destination, MeshImportSettings settings);

        template<typename T>
        static T GetImportSettings(const std::filesystem::path& path)
        {
            __debugbreak();
            //static_assert(false);
        }

        template<>
        static ImageImportSettings GetImportSettings(const std::filesystem::path& path)
        {
            using namespace HazardRenderer;

            ImageImportSettings settings = {};

            if (!File::Exists(path.lexically_normal().string() + ".meta"))
            {
                settings.MinFilter = FilterMode::Linear;
                settings.MagFilter = FilterMode::Linear;
                settings.Wrapping = ImageWrap::ClampBorder;
                settings.FlipOnLoad = true;
                settings.GenerateMips = false;
                return settings;
            }

            YAML::Node root = YAML::LoadFile(path.lexically_normal().string() + ".meta");


            if (root["Importer"])
            {
                auto node = root["Importer"];
                uint32_t wrapping, minFilter, magFilter;

                YamlUtils::Deserialize(node, "WrapMode", wrapping, (uint32_t)ImageWrap::ClampBorder);
                YamlUtils::Deserialize(node, "MinFilter", minFilter, (uint32_t)FilterMode::Linear);
                YamlUtils::Deserialize(node, "MagFilter", magFilter, (uint32_t)FilterMode::Linear);
                YamlUtils::Deserialize(node, "Mipmaps", settings.GenerateMips, false);
                YamlUtils::Deserialize(node, "FlipOnLoad", settings.FlipOnLoad, true);

                settings.Wrapping = (ImageWrap)wrapping;
                settings.MinFilter = (FilterMode)minFilter;
                settings.MagFilter = (FilterMode)magFilter;
            }
            return settings;
        }
        template<>
        static MeshImportSettings GetImportSettings(const std::filesystem::path& path)
        {
            using namespace HazardRenderer;

            MeshImportSettings settings = {};

            if (!File::Exists(path.lexically_normal().string() + ".meta"))
            {
                settings.Scale = 1.0f;
                return settings;
            }

            YAML::Node root = YAML::LoadFile(path.lexically_normal().string() + ".meta");

            if (root["Importer"])
            {
                auto node = root["Importer"];
                YamlUtils::Deserialize(node, "Scale", settings.Scale, 1.0f);
            }
            return settings;
        }

    private:
        std::filesystem::path m_CurrentFilePath;
        AssetType m_AssetType = AssetType::Undefined;

        Buffer m_ImportDataBuffer;
        AssetHandle m_AssetHandle;

        bool m_Import = false;
        bool m_IsImportingNew = false;
    };
}
