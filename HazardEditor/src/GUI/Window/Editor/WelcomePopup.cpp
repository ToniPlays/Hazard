#pragma once
#include <hzreditor.h>
#include "WelcomePopup.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/EditorView.h"
#include "GUI/Library/Input.h"
#include "Project/ProjectManager.h"

namespace WindowElement {

	WelcomePopup::WelcomePopup() : EditorPopup("Welcome")
	{
		m_Size = { 480, 280 };
	}
	void WelcomePopup::OnWindowRender()
	{
		ImGui::Text("Hazard engine is ready to be used");
		WindowLayout::Layout::NextLine(10);
		ImGui::Text("Feel free to crash this very good game engine that can't do anything");

		float width = ImGui::GetWindowWidth() / 2;
		float height = ImGui::GetWindowHeight();
		ImGui::SetCursorPosX(width - 200 / 2);
		ImGui::SetCursorPosY(height - 80 - 15);

		if (Input::Button("Load project", { 200, 80 })) {
			std::string path = File::OpenFileDialog("Hazard project (*.hzrproj)\0*.hzrproj\0");
			if (path != "") {
				Application::GetModule<Project::ProjectManager>().Load(path);
				EditorView::GetInstance().RemoveRenderable<WelcomePopup>();
			}
		}
	}
	bool WelcomePopup::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(WelcomePopup::Close));
	}
	bool WelcomePopup::Close(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::Escape) {
			EditorView::GetInstance().RemoveRenderable<WelcomePopup>();
			return true;
		}
		return false;
	}
}
