#pragma once

#include "Hazard/ImGUI/Modal.h"
#include "Hazard/Assets/Asset.h"
#include "Utility/YamlUtils.h"
#include "AssetImporters/IAssetImporter.h"

namespace UI
{
    class AssetImporterPanel : public Hazard::ImUI::Modal
    {
    public:
        AssetImporterPanel();
        ~AssetImporterPanel() = default;

        void Update() override {};
        bool OnEvent(Event& e) override { return false; };
        void OnPanelRender() override;

        void Open(AssetHandle handle);
        void Open(const std::filesystem::path& sourceFile);

        template<typename T, typename... Args>
        void RegisterImporter(AssetType type, Args&&... args)
        {
            m_Importers[type] = CreateScope<T>(std::forward<Args>(args)...);
        }
    private:
        void DrawTopBar();
        void DrawBottomBar();

    private:
        std::unordered_map<AssetType, Scope<IAssetImporter>> m_Importers;
        AssetType m_CurrentImportType;
        std::filesystem::path m_Source;
    };
}
