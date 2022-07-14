#pragma once

#include "GUI/Core/Panel.h"

namespace UI {
	class RenderCommandListPanel : public Panel {
	public:
		RenderCommandListPanel() : Panel("RenderCommandList") {};
		
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };
	};
}