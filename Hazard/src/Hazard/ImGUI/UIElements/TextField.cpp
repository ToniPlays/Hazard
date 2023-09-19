#include "TextField.h"
#include "../StyleManager.h"
#include "../UILibrary.h"
#include <Hazard/ImGUI/ScopedVar.h>

namespace Hazard::ImUI
{
	void TextField::Render()
	{
		constexpr float fieldHeight = 28.0f;
		constexpr float buttonHeight = 22.0f;
		const Style& style = StyleManager::GetCurrent();

		char buffer[512] = { 0 };
		strcpy(buffer, m_Value.c_str());

		ImVec2 contentSize = ImGui::GetContentRegionAvail();
		ScopedColourStack bgColor(ImGuiCol_ChildBg, style.Frame.FrameColor, ImGuiCol_Button, style.Frame.FrameColor);
		ScopedStyleVar rounding(ImGuiStyleVar_ChildRounding, style.Frame.Rounding);
		ImGui::BeginChild("#textField", { contentSize.x, fieldHeight }, false, 0);
		{
			ImUI::Shift(4.0f, (fieldHeight - buttonHeight) * 0.5f);
			if (!m_Icon.empty())
			{
				ScopedStyleStack padding(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
				if (ImGui::Button(m_Icon.c_str(), { buttonHeight, buttonHeight }))
				{

				}
				ImGui::PopFont();
				ImGui::SameLine();
			}
			{
				ScopedStyleStack padding(ImGuiStyleVar_FramePadding, ImVec2(0, (buttonHeight - GImGui->Font->FontSize) * 0.5f));
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - buttonHeight - 4.0f);
				if (m_Hint.empty())
					m_DidChange = ImGui::InputText("##text", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue, NULL, nullptr);
				else
					m_DidChange = ImGui::InputTextWithHint("##text", m_Hint.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue, NULL, nullptr);
			}

			ImGui::SameLine();

			{
				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
				ImGui::SetCursorPosX(contentSize.x - buttonHeight - 4);
				ScopedStyleStack padding(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

				if (ImGui::Button((const char*)ICON_FK_TIMES, { buttonHeight, buttonHeight }))
				{
					Clear();
				}
				ImGui::PopFont();
			}

		}
		ImGui::EndChild();
		m_Value = buffer;
	}
}