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
	void EditorWindow::RenderWindow()
	{
		if (GetActive()) {
			ImGui::Begin(title.c_str(), &active);
			OnWindowRender();
			ImGui::End();
		}
	}
}