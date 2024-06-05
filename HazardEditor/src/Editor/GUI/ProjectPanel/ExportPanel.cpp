#include "ExportPanel.h"
#include "Hazard/Assets/AssetManager.h"
#include "Project/ProjectManager.h"

using namespace Hazard;

namespace UI
{
    ExportPanel::ExportPanel() : Hazard::ImUI::Panel("Export")
    {
        m_SelectedWorldTable.SetName("BuildWorldTable");
        m_SelectedWorldTable.SetColumns({ "Build index", "Name" });
        m_SelectedWorldTable.RowHeight(32);
    }

    void ExportPanel::OnPanelRender()
    {
        m_SelectedWorldTable.Size({ ImGui::GetContentRegionAvail().x, 250 });
        m_SelectedWorldTable.Render();

        if (ImGui::Button("Export", { 150, 60 }))
        {
            auto& project = ProjectManager::GetCurrentProject();
            auto& info = project.GetSettings();

            std::vector<AssetHandle> handles;
            handles.reserve(m_SelectedWorldTable.GetRows().size());

            for (auto& metadata : m_SelectedWorldTable.GetRows())
                handles.push_back(metadata.Handle);
        }
    }
}
