#pragma once

#include "Hazard.h"
#include "Hazard/ImGUI/Panel.h"

namespace UI
{
	class RenderCommandListPanel : public Hazard::ImUI::Panel
	{
	public:
		RenderCommandListPanel() : Hazard::ImUI::Panel("RenderCommandList") {}

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };

		void DrawDetailedInfo(Hazard::RendererDrawList& drawList);
	};
}