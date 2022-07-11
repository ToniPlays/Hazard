#pragma once

#include "GUI/Core/Panel.h"

namespace UI {
	class Hierarchy : public Panel {
	public:
		Hierarchy() : Panel("Hierarchy") {};

		void OnPanelRender() override;
	};
}