#pragma once
#include "Hazard.h"

namespace UI
{
    class ExportPanel : public Hazard::ImUI::Panel
    {
    public:
        ExportPanel() : Hazard::ImUI::Panel("Export") { m_Open = false; };

        void Update() override {};
        void OnOpen() override;
        void OnPanelRender() override;
        bool OnEvent(Event& e) override { return false; };
        
    private:
        std::vector<Hazard::AssetMetadata> m_SelectedScenes;
    };
}
