#include <hzrpch.h>

#include "InputColor.h"
#include <Hazard/ImGUI/ScopedVar.h>
#include "../StyleManager.h"
#include "../UILibrary.h"

namespace Hazard::ImUI
{
	void InputColor::Render()
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
		float itemWidth = 4.0f - buttonSize.x;

		m_DidChange = ColorPicker("#picker", m_Color);

		if (!m_Title.empty())
			ImGui::Columns();

		ImGui::PopID();
	}
}
