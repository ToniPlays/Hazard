#pragma once
#include <hzreditor.h>
#include "WelcomePopup.h"
#include "GUI/Library/Layout/Layout.h"

namespace WindowElement {

	WelcomePopup::WelcomePopup() : EditorPopup("Welcome")
	{
		m_Size = { 480, 280 };
	}
	WelcomePopup::~WelcomePopup()
	{

	}
	void WelcomePopup::OnWindowRender()
	{
		ImGui::Text("Hazard engine is ready to be used");
		WindowLayout::Layout::NextLine(10);
		ImGui::Text("Feel free to crash this very good game engine that can't do anything");
	}
	bool WelcomePopup::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(WelcomePopup::Close));
	}
	bool WelcomePopup::Close(KeyPressedEvent& e)
	{
		
		if (e.GetKeyCode() == Key::Escape) {
			open = false;
			return true;
		}
		return false;
	}
}
