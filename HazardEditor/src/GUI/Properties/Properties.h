#pragma once

#include "GUI/Core/Panel.h"

namespace UI {
	class Properties : public Panel {
	public:
		Properties();

		void OnPanelRender() override;
		bool OnEvent(Event& e) override;
		bool OnSelectionContextChange(Events::SelectionContextChange& e);
	private:
		Entity m_SelectionContext;
	};
}