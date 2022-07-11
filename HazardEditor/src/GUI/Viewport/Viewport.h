#pragma once

#include "GUI/Core/Panel.h"

namespace UI {
	class Viewport : public Panel {
	public:
		Viewport() : Panel("Viewport") {};

		void OnPanelRender() override;
	};
}