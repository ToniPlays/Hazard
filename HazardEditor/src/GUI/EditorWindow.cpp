#pragma once

#include <hzreditor.h>
#include "EditorWindow.h"

namespace WindowElement {

	EditorWindow::EditorWindow()
	{

	}
	EditorWindow::EditorWindow(const char* title) : title(title)
	{
		
	}
	EditorWindow::~EditorWindow()
	{

	}
	void EditorWindow::OnRender()
	{
		if (GetActive()) {
			ImGui::Begin(title.c_str(), &active);

			OnWindowRender();

			focused = ImGui::IsWindowFocused();
			hovered = ImGui::IsWindowHovered();
			ImGui::End();
		}
	}
}