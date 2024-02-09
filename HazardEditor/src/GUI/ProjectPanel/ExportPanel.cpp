#include "ExportPanel.h"
#include "Hazard/Assets/AssetManager.h"
#include "Project/ProjectManager.h"

using namespace Hazard;

namespace UI
{
    ExportPanel::ExportPanel() : Hazard::ImUI::Panel("Export", false)
    {
        m_SelectedWorldTable.SetName("BuildWorldTable");
        m_SelectedWorldTable.SetColumns({ "Build index", "Name" });
        m_SelectedWorldTable.RowHeight(32);
    }

    void ExportPanel::OnOpen()
    {
        m_SelectedWorldTable.ClearRows();

        /*auto metadatas = AssetManager::GetAllAssetMetadata(AssetType::World);
        m_SelectedWorldTable.Reserve(metadatas.size());

        for (auto& metadata : metadatas)
            m_SelectedWorldTable.AddRow(metadata);

        m_SelectedWorldTable.RowContent([](uint32_t index, AssetMetadata& metadata) {
            ImUI::Shift(4.0f, 4.0f);
            ImGui::Text("%u", index);
            ImGui::TableNextColumn();

            ImUI::Shift(4.0f, 4.0f);
            ImGui::Text("%s", metadata.Key.c_str());
        });
        */
    }
    void ExportPanel::OnPanelRender()
    {
        m_SelectedWorldTable.Size({ ImGui::GetContentRegionAvail().x, 250 });
        m_SelectedWorldTable.Render();

        bool disabled = m_Exporter.IsExporting();
        if (disabled)
            ImGui::BeginDisabled();

        if (ImGui::Button("Export", { 150, 60 }))
        {
            auto& project = ProjectManager::GetProject();
            auto& info = project.GetInfo();

            std::vector<AssetHandle> handles;
            handles.reserve(m_SelectedWorldTable.GetRows().size());

            for (auto& metadata : m_SelectedWorldTable.GetRows())
                handles.push_back(metadata.Handle);

            Exporter::ExportSettings settings = {};
            settings.TargetPath = info.ProjectPath / fmt::format("Builds/{0}-{1}", info.ProjectName, info.ProjectVersion);
            settings.WorldHandles = handles;

            m_Exporter.Export(settings);
        }

        if (disabled)
            ImGui::EndDisabled();
    }
}
