#pragma once

#include "GUI/EditorPopup.h"
#include "imgui.h"

namespace WindowElement {

	class WelcomePopup : public EditorPopup {
	public:
		WelcomePopup();
		~WelcomePopup() = default;

		void OnWindowRender() override;
		//virtual bool OnEvent(Hazard::Event& e) override;

	private:
		//bool Close(Hazard::KeyPressedEvent& e);

	private:
		const char* m_Title = "Popup";
	};
}
