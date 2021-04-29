#pragma once

#include "GUI/EditorPopup.h"
#include "imgui.h"

namespace WindowElement {

	class WelcomePopup : public EditorPopup {
	public:
		WelcomePopup();
		~WelcomePopup();

		void OnWindowRender() override;
		virtual void Init() {};
		virtual bool OnEvent(Event& e) override;

	private:
		bool Close(KeyPressedEvent& e);

	private:
		const char* m_Title = "Popup";
	};
}