#pragma once

#include "Hazard.h"
#include "Hazard/ImGUI/Panel.h"

namespace UI
{
	class RendererDebugPanel : public Hazard::ImUI::Panel
	{
	public:
		RendererDebugPanel() : Hazard::ImUI::Panel("RendererDebug") {};

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };
	};
}