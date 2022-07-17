#pragma once

#include "GUI/Core/Panel.h"

namespace UI {
	class RendererDebugPanel : public Panel {
	public:
		RendererDebugPanel() : Panel("RendererDebug") { m_Open = false; };

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };
	};
}