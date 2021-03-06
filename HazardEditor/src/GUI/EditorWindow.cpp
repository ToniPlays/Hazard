#pragma once

#include <hzreditor.h>
#include "GUI/Library/Style.h"
#include "EditorWindow.h"

namespace WindowElement {

	EditorWindow::EditorWindow(const char* title, ImGuiWindowFlags flags) : m_Title(title), m_Flags(flags)
	{
		
	}

	EditorWindow::~EditorWindow()
	{

	}
	void EditorWindow::OnRender()
	{
		if (GetActive()) {
			OnBeforeRender();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, Style::GetStyle().framePadding);
			ImGui::Begin(m_Title.c_str(), &m_Active, m_Flags);
			ImGui::PopStyleVar();
			m_Focused = ImGui::IsWindowFocused();
			m_Hovered = ImGui::IsWindowHovered();
			OnWindowRender();
			ImGui::End();
			OnAfterRender();
		}
	}
}