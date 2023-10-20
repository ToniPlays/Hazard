#include "InputFloat.h"
#include <Hazard/ImGUI/ScopedVar.h>
#include "../StyleManager.h"
#include "../UILibrary.h"

namespace Hazard::ImUI
{
	void InputFloat::Render()
	{
		ImGui::PushID(m_Title.c_str());

		if (!m_Title.empty())
		{
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, 125);
			ImUI::ShiftY(4.0f);
			ImGui::Text("%s", m_Title.c_str());
			ImUI::ShiftY(4.0f);
			ImGui::NextColumn();
		}

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ScopedStyleVar padding(ImGuiStyleVar_FrameBorderSize, 0.0f);
		ScopedStyleVar spacing(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 0));
		float itemWidth = (ImGui::GetContentRegionAvail().x - 4.0f * m_Configs.size()) / (float)m_Configs.size() - buttonSize.x;

		for (uint32_t i = 0; i < m_Configs.size(); i++)
		{
			DrawInputField(i, buttonSize, itemWidth);
			if (i < m_Configs.size() - 1)
				ImGui::SameLine();
		}

		if (!m_Title.empty())
			ImGui::Columns();

		ImGui::PopID();
	}
	void InputFloat::DrawInputField(uint32_t index, ImVec2 buttonSize, float itemWidth)
	{
		auto& config = m_Configs[index];
		ImFont* font = ImGui::GetIO().Fonts->Fonts[1];

		ImVec4 hovered = ImGui::ColorConvertU32ToFloat4(ColorWithMultiplier(config.Color, 0.9f));
		ImVec4 active = ImGui::ColorConvertU32ToFloat4(ColorWithMultiplier(config.Color, 0.8f));
		ScopedColorStack colors(ImGuiCol_Button, config.Color, ImGuiCol_ButtonHovered, hovered, ImGuiCol_ButtonActive, active);


		ImGui::PushFont(font);
		if (ImGui::Button(config.ButtonText.c_str(), buttonSize))
		{
			m_Flags |= BIT(index);
			config.Value = config.DefaultValue;
			config.DidChange = true;
		}

		ImGui::PopFont();
		ImGui::SameLine();

		std::stringstream ss;
		ss << "##" << config.ButtonText << "_";
		ImGui::SetNextItemWidth(itemWidth);

		ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, config.Mixed);
		if (ImGui::DragFloat(ss.str().c_str(), &config.Value))
		{
			m_Flags |= BIT(index);
			config.DidChange = true;
		}

		ImGui::PopItemFlag();
	}
}
