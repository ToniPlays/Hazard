#pragma once

#include "Hazard.h"

namespace UI {
	class MainMenuBar : public Hazard::ImUI::MenuBar {
	public:
		MainMenuBar();
		~MainMenuBar() = default;

		void Init() override;
		void Update() override {};

		virtual bool OnEvent(Event& e) { return false; }
	};
}