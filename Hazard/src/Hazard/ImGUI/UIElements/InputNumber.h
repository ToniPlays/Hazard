#pragma once

#include "UtilityCore.h"
#include "Buffer/Buffer.h"
#include "Color.h"
#include <imgui_internal.h>
#include "Hazard/ImGUI/StyleManager.h"

namespace Hazard::ImUI
{
	template<typename NumType>
	class InputNumber
	{
	public:
		InputNumber(const std::string& title, NumType value, bool slider = false) : m_Title(title), m_Value(value), m_Slider(slider) {};
		InputNumber(const char* title, NumType value) : m_Title(title), m_Value(value) {};
		~InputNumber() {}

		void Render()
		{
			using namespace Hazard::ImUI;

			ImGui::PushID(m_Title.c_str());
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, 100.0f);
			ImUI::Shift(0.0f, 4.0f);
			ImGui::Text("%s", m_Title.c_str());
			ImGui::NextColumn();

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

			if constexpr (std::is_same<NumType, float>::value)
			{
				if (m_Slider) m_DidChange = ImGui::SliderFloat("##val", &m_Value, m_Min, m_Max);
				else m_DidChange = ImGui::DragFloat("##val", &m_Value, 0.25f, m_Min, m_Max);
			}

			ImGui::Columns();
			ImGui::PopID();
		}
		void SetRange(NumType min, NumType max) { m_Min = min; m_Max = max; }
		bool DidChange() { return m_DidChange; }
		NumType GetValue() { return m_Value; }

	private:
		std::string m_Title;
		NumType m_Value;
		NumType m_Min;
		NumType m_Max;
		bool m_DidChange = false;
		bool m_Slider = false;
	};
}
