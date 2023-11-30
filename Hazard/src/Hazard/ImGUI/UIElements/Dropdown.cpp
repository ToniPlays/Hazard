#include "Dropdown.h"
#include "../StyleManager.h"
#include "../UILibrary.h"
#include <Hazard/ImGUI/ScopedVar.h>

namespace Hazard::ImUI
{
	void Dropdown::Render()
	{
		const Style& style = StyleManager::GetCurrent();
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, m_Width);
		ImGui::Text("%s", m_Title.c_str());
		ImGui::NextColumn();

		ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
		ScopedStyleVar rounding(ImGuiStyleVar_FrameRounding, style.Frame.Rounding);

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, m_Mixed);

		if (ImGui::BeginCombo(("##" + m_Title).c_str(), m_Options[m_Selected].c_str(), 0))
		{
			for (uint32_t i = 0; i < m_Options.size(); i++)
			{
				bool isSelected = i == m_Selected;

				if (ImGui::Selectable(m_Options[i].c_str(), i == m_Selected))
				{
					m_Selected = i;
					m_DidChange = true;
					isSelected = true;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemFlag();
		ImGui::Columns();
	}
}
