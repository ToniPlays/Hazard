#pragma once

#include "Core/MenuBar.h"
#include "Hazard.h"

namespace UI {
	class MainMenuBar : public MenuBar {
	public:
		MainMenuBar();
		~MainMenuBar() = default;

		void Update() override {};

		virtual bool OnEvent(Event& e) { return false; }
	};
}