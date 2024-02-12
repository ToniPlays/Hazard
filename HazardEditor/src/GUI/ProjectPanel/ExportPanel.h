#pragma once
#include "Hazard.h"
#include "Hazard/ImGUI/UIElements/Table.h"

namespace UI
{
    class ExportPanel : public Hazard::ImUI::Panel
    {
    public:
        ExportPanel();

        void Update() override {};
        void OnOpen() override;
        void OnPanelRender() override;
        bool OnEvent(Event& e) override { return false; };
        
    private:
        Hazard::ImUI::Table<Hazard::AssetMetadata> m_SelectedWorldTable;
    };
}
