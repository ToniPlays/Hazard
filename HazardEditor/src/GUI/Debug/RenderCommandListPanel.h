#pragma once

#include "Hazard.h"

namespace UI {
	class RenderCommandListPanel : public Hazard::ImUI::Panel {
	public:
		RenderCommandListPanel() : Hazard::ImUI::Panel("RenderCommandList") { m_Open = true; };
		
		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };
	};
}