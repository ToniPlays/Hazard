
#include "Library/Style.h"
#include "EditorWindow.h"

namespace WindowElement {

	EditorWindow::EditorWindow(const char* title, ImGuiWindowFlags flags) : m_Title(title), m_Flags(flags)
	{

	}

	void EditorWindow::OnRender()
	{
		if (IsActive()) {
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
