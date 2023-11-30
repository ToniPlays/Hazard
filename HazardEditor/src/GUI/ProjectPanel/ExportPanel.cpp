#include "ExportPanel.h"
#include "Hazard/Assets/AssetManager.h"

using namespace Hazard;

namespace UI
{
    void ExportPanel::OnOpen() 
    {
        m_SelectedScenes = AssetManager::GetAllAssetMetadata(AssetType::World);
    }
    void ExportPanel::OnPanelRender()
    {
        for(auto& metadata : m_SelectedScenes)
        {
            ImGui::Text("%s", metadata.Key.c_str());
        }
    }
}
