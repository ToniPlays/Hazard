#pragma once

#include "Hazard.h"

namespace UI {
	class RendererDebugPanel : public Hazard::ImUI::Panel{
	public:
		RendererDebugPanel() : Hazard::ImUI::Panel("RendererDebug") { m_Open = false; };

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };
	};
}