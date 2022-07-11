#pragma once

#include "GUI/Core/Panel.h"

namespace UI {
	class Properties : public Panel {
	public:
		Properties() : Panel("Properties") {};

		void OnPanelRender() override;
	};
}