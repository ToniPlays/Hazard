#pragma once

#include "GUI/Core/Panel.h"

namespace UI {
	class AssetPanel : public Panel {
	public:
		AssetPanel() : Panel("AssetPanel") {};

		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };
	};
}