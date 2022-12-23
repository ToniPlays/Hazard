#pragma once

#include "Hazard.h"
#include "HazardRendererCore.h"

namespace UI {
	class MainMenuBar : public Hazard::ImUI::MenuBar {
	public:
		MainMenuBar();
		~MainMenuBar() = default;

		void Init() override;
		void Update() override {};

		virtual bool OnEvent(Event& e) override;
		bool OnKeyPressed(KeyPressedEvent& e);
	};
}
