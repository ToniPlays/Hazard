#pragma once

#include <imgui.h>
#include <iostream>

namespace Hazard::ImUI
{
	inline static ImU32 ColorWithMultiplier(const ImVec4& color, float multiplier)
	{
		const ImVec4& colRow = color;
		float hue, sat, val;
		ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
		return ImColor::HSV(hue, sat, std::min(val * multiplier, 1.0f));
	}

	template<typename Value>
	class ScopedStyleVar 
	{
	public:
		ScopedStyleVar(ImGuiStyleVar var, Value value) 
		{
			ImGui::PushStyleVar(var, value);
		}
		~ScopedStyleVar() 
		{
			ImGui::PopStyleVar();
		}
	};
	template<typename T>
	class ScopedStyleColor 
	{
	public:
		ScopedStyleColor(ImGuiStyleVar var, T value) 
		{
			ImGui::PushStyleColor(var, value);
		}
		~ScopedStyleColor() 
		{
			ImGui::PopStyleColor();
		}
	};
	class ScopedFont {
	public:
		ScopedFont(ImFont* font) 
		{
			ImGui::PushFont(font);
		}
		~ScopedFont() 
		{
			ImGui::PopFont();
		}
	};
	class ScopedColorStack
	{
	public:
		ScopedColorStack(const ScopedColorStack&) = delete;
		ScopedColorStack operator=(const ScopedColorStack&) = delete;

		template <typename ColourType, typename... OtherColours>
		ScopedColorStack(ImGuiCol firstColourID, ColourType firstColour, OtherColours&& ... otherColourPairs)
			: m_Count((sizeof... (otherColourPairs) / 2) + 1)
		{
			static_assert ((sizeof... (otherColourPairs) & 1u) == 0,
				"ScopedColourStack constructor expects a list of pairs of colour IDs and colours as its arguments");

			PushColour(firstColourID, firstColour, std::forward<OtherColours>(otherColourPairs)...);
		}

		~ScopedColorStack() {
			ImGui::PopStyleColor(m_Count);
		}

	private:
		int m_Count;

		template <typename ColourType, typename... OtherColours>
		void PushColour(ImGuiCol colourID, ColourType colour, OtherColours&& ... otherColourPairs)
		{
			if constexpr (sizeof... (otherColourPairs) == 0)
			{
				ImGui::PushStyleColor(colourID, colour);
			}
			else
			{
				ImGui::PushStyleColor(colourID, colour);
				PushColour(std::forward<OtherColours>(otherColourPairs)...);
			}
		}
	};

	class ScopedStyleStack
	{
	public:
		ScopedStyleStack(const ScopedStyleStack&) = delete;
		ScopedStyleStack operator=(const ScopedStyleStack&) = delete;

		template <typename ValueType, typename... OtherStylePairs>
		ScopedStyleStack(ImGuiStyleVar firstStyleVar, ValueType firstValue, OtherStylePairs&& ... otherStylePairs)
			: m_Count((sizeof... (otherStylePairs) / 2) + 1)
		{
			static_assert ((sizeof... (otherStylePairs) & 1u) == 0,
				"ScopedStyleStack constructor expects a list of pairs of colour IDs and colours as its arguments");

			PushStyle(firstStyleVar, firstValue, std::forward<OtherStylePairs>(otherStylePairs)...);
		}

		~ScopedStyleStack() {
			ImGui::PopStyleVar(m_Count);
		}

	private:
		int m_Count;

		template <typename ValueType, typename... OtherStylePairs>
		void PushStyle(ImGuiStyleVar styleVar, ValueType value, OtherStylePairs&& ... otherStylePairs)
		{
			if constexpr (sizeof... (otherStylePairs) == 0)
			{
				ImGui::PushStyleVar(styleVar, value);
			}
			else
			{
				ImGui::PushStyleVar(styleVar, value);
				PushStyle(std::forward<OtherStylePairs>(otherStylePairs)...);
			}
		}
	};
}