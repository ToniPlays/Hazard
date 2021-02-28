#pragma once
#include <hzreditor.h>
#include "Input.h"
#include "Style.h"
#include "Layout.h"

using namespace WindowLayout;

namespace WindowElement {

	bool Input::InputField(std::string& text)
	{
		char buffer[512];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), text.c_str());

		if (ImGui::InputText("##InputField", buffer, sizeof(buffer)))
		{
			text = buffer;
			return true;
		}
		return false;
	}
	bool Input::Button(const char* name, ImVec2 size)
	{
		return ImGui::Button(name, size);
	}
	void Input::ResettableDragButton(const char* label, float& value, float resetValue, ImVec2 size, uint16_t buttonFont, uint16_t dragFont)
	{
		Style::Style::SelectFont(buttonFont);
		
		if (Button(label, size)) {
			value = resetValue;
		}

		ImGui::PopFont();
		Style::Style::SelectFont(dragFont);

		ImGui::SameLine();
		std::stringstream ss;
		ss << "##" << label;
		Layout::MaxWidth();
		ImGui::DragFloat(ss.str().c_str(), &value, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopFont();
	}
	void Input::Vec1(const char* label, float& value, float resetValue, float columnWidth)
	{
		Layout::Table(2, false);
		Layout::SetColumnWidth(columnWidth);
		Layout::Text(label);
		Layout::TableNext();

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		Style::Style::SetButtonColors("#DB3721", "#C3311D", "#A02818");
		Input::ResettableDragButton("X", value, resetValue, buttonSize, 1, 0);
		ImGui::PopStyleColor(3);
		
		Layout::EndTable();
	}
	void Input::Vec2(const char* label, glm::vec2& value, float resetValue, float columnWidth)
	{
		Layout::Table(3, false);
		Layout::SetColumnWidth(columnWidth);
		Layout::Text(label);
		Layout::TableNext();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		Style::Style::SetButtonColors("#DB3721", "#C3311D", "#A02818");
		Input::ResettableDragButton("X", value.x, resetValue, buttonSize, 1, 0);
		ImGui::PopStyleColor(3);
		Layout::TableNext();

		Style::Style::SetButtonColors("#53B305", "#4A9F04", "#418B04");
		Input::ResettableDragButton("Y", value.y, resetValue, buttonSize, 1, 0);
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();

		Layout::EndTable();
	}
	void Input::Vec3(const char* label, glm::vec3& value, float resetValue, float columnWidth)
	{
		Layout::Table(4, false);
		Layout::SetColumnWidth(columnWidth);
		Layout::Text(label);
		Layout::TableNext();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		
		Style::Style::SetButtonColors("#DB3721", "#C3311D", "#A02818");
		Input::ResettableDragButton("X", value.x, resetValue, buttonSize, 1, 0);
		ImGui::PopStyleColor(3);
		Layout::TableNext();

		Style::Style::SetButtonColors("#53B305", "#4A9F04", "#418B04");
		Input::ResettableDragButton("Y", value.y, resetValue, buttonSize, 1, 0);
		ImGui::PopStyleColor(3);
		Layout::TableNext();

		Style::Style::SetButtonColors("#1651F3", "#0B41D5", "#0935AE");
		Input::ResettableDragButton("Z", value.z, resetValue, buttonSize, 1, 0);
		ImGui::PopStyleColor(3);
		
		ImGui::PopStyleVar();

		Layout::EndTable();
	}
	void Input::Checkbox(const char* label, bool& value)
	{
		ImGui::Checkbox(label, &value);
	}
	void Input::ColorPicker(const char* label, Hazard::Color& color)
	{
		static bool open = false;
		if (ImGui::ColorButton(label, Style::Style::ColorAsImVec4(color))) {
			open = true;
		}
		if (open) {
			float c[4] = { color.r, color.g, color.b, color.a };
			ImGui::Begin(label, &open);

			if (ImGui::ColorPicker4(label, c)) {
				color.r = c[0];
				color.g = c[1];
				color.b = c[2];
				color.a = c[3];
			}
			ImGui::End();
		}
	}
}