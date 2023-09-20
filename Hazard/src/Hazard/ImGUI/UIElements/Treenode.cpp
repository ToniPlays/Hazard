#include "Treenode.h"
#include "../UILibrary.h"
#include "../StyleManager.h"
#include "../ScopedVar.h"

namespace Hazard::ImUI
{
	void Treenode::Render()
	{
		const Style& style = StyleManager::GetCurrent();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, style.Frame.Padding);

		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

		bool open = false;

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		open |= ImGui::TreeNodeEx(m_Title.c_str(), m_Flags | ImGuiTreeNodeFlags_SpanAvailWidth | (m_MenuIcon.empty() ? 0 : ImGuiTreeNodeFlags_AllowItemOverlap));
		ImGui::PopStyleVar();

		if (!m_MenuIcon.empty())
		{
			ImGui::SameLine(contentRegion.x - lineHeight * 0.5f - 12);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0, 0 });

			ScopedColourStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);

			if (ImGui::ButtonEx(m_MenuIcon.c_str(), ImVec2 { lineHeight, lineHeight }, ImGuiButtonFlags_PressedOnClick))
				ImGui::OpenPopup(m_Title.c_str());
			ImGui::PopStyleVar();

			ScopedStyleStack padding(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f), ImGuiStyleVar_PopupRounding, 4.0f, ImGuiStyleVar_ItemSpacing, ImVec2(16.0f, 4.0f), ImGuiStyleVar_ChildBorderSize, 0);
			
			if (ImGui::BeginPopup(m_Title.c_str()))
			{
				m_MenuCallback();
				ImGui::EndPopup();
			}
		}

		if (open)
		{
			m_ContentCallback();
			ImGui::TreePop();
		}
	}
}
