#pragma once

#include "Hazard.h"
#include "Hazard/Assets/Asset.h"
#include "Hazard/ImGUI/Panel.h"

struct ImColor;

namespace UI
{
    class ScriptDebugPanel : public Hazard::ImUI::Panel
    {
    public:
        ScriptDebugPanel() : Hazard::ImUI::Panel("ScriptDebugPanel") {}

        void Update() override {};
        void OnPanelRender() override;
        bool OnEvent(Event& e) override { return false; };

    private:
        std::string m_SearchValue;
    };
}
