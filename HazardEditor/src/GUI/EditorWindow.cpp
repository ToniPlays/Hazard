#pragma once

#include <hzreditor.h>
#include "GUI/Library/Style.h"
#include "EditorWindow.h"

namespace WindowElement {

	EditorWindow::EditorWindow()
	{

	}

	EditorWindow::EditorWindow(const char* title) : m_Title(title)
	{
		
	}

	EditorWindow::~EditorWindow()
	{

	}

	void EditorWindow::OnRender()
	{
		if (GetActive()) {
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, Appereance::Style::GetStyle().framePadding);
			ImGui::Begin(m_Title.c_str(), &m_Active);
			m_Focused = ImGui::IsWindowFocused();
			m_Hovered = ImGui::IsWindowHovered();
			ImGui::PopStyleVar();

			OnWindowRender();
			ImGui::End();
		}
	}
}