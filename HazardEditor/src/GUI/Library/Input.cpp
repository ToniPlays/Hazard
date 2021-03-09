#pragma once
#include <hzreditor.h>
#include "Input.h"
#include "Style.h"
#include "Layout.h"

#include "imgui.h"
#include "imgui_internal.h"

using namespace WindowLayout;
using namespace Appereance;

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
	bool Input::ResettableDragButton(const char* label, float& value, float resetValue, ImVec2 size, uint16_t buttonFont, uint16_t dragFont)
	{
		bool modified = false;
		Style::SelectFont(buttonFont);
		
		if (Button(label, size)) {
			value = resetValue;
			modified = true;
		}

		ImGui::PopFont();
		Style::SelectFont(dragFont);

		std::stringstream ss;
		ss << "##" << label;
		Layout::SameLine();

		if (ImGui::DragFloat(ss.str().c_str(), &value, 0.1f, 0.0f, 0.0f, "%.2f")) 
			modified = true;
		ImGui::PopFont();

		return modified;
	}
	bool Input::Vec1(const char* label, float& value, float resetValue, float columnWidth)
	{

		bool modified = false;
		Layout::Table(2, false);
		Layout::SetColumnWidth(columnWidth);
		Layout::Text(label);
		Layout::TableNext();

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		Style::SetButtonColors("#DB3721", "#C3311D", "#A02818");
		if (Input::ResettableDragButton("X", value, resetValue, buttonSize, 1, 0)) modified = true;
		ImGui::PopStyleColor(3);
		
		Layout::EndTable();

		return modified;
	}
	bool Input::Vec2(const char* label, glm::vec2& value, float resetValue, float columnWidth)
	{
		bool modified = true;
		auto boldFont = ImGui::GetIO().Fonts->Fonts[1];

		Layout::Table(2, false);
		Layout::SetColumnWidth(columnWidth);
		Layout::Text(label);
		Layout::TableNext();

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		Style::SetButtonColors("#DB3721", "#C3311D", "#A02818");
		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", buttonSize)) {
			modified = true;
			value.x = resetValue;
		}
		ImGui::PopFont();
		Layout::SameLine();
		if (Input::DragFloat("##x", value.x)) modified = true;
		ImGui::PopStyleColor(3);


		Layout::SameLine();
		Style::SetButtonColors("#53B305", "#4A9F04", "#418B04");
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Y", buttonSize)) {
			modified = true;
			value.y = resetValue;
		}
		ImGui::PopFont();
		Layout::SameLine();
		if (Input::DragFloat("##Y", value.y)) modified = true;
		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
		Layout::EndTable();

		return modified;
	}
	bool Input::Vec3(const char* label, glm::vec3& value, float resetValue, float columnWidth)
	{
		bool modified = true;
		auto boldFont = ImGui::GetIO().Fonts->Fonts[1];

		Layout::Table(2, false);
		Layout::SetColumnWidth(columnWidth);
		Layout::Text(label);
		Layout::TableNext();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		
		Style::SetButtonColors("#DB3721", "#C3311D", "#A02818");
		ImGui::PushFont(boldFont);
		
		if (ImGui::Button("X", buttonSize)) {
			modified = true;
			value.x = resetValue;
		}
		ImGui::PopFont();
		Layout::SameLine();
		if (Input::DragFloat("##x_", value.x)) modified = true;
		ImGui::PopStyleColor(3);


		Layout::SameLine();
		Style::SetButtonColors("#53B305", "#4A9F04", "#418B04");
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Y", buttonSize)) {
			modified = true;
			value.y = resetValue;
		}
		ImGui::PopFont();
		Layout::SameLine();
		if (Input::DragFloat("##Y_", value.y)) modified = true;
		ImGui::PopStyleColor(3);


		Layout::SameLine();
		Style::SetButtonColors("#1651F3", "#0B41D5", "#0935AE");
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Z", buttonSize)) {
			modified = true;
			value.z = resetValue;
		}
		ImGui::PopFont();
		Layout::SameLine();
		if (Input::DragFloat("##z_", value.z)) modified = true;
		ImGui::PopStyleColor(3);


		ImGui::PopStyleVar();
		Layout::EndTable();

		return modified;
	}
	bool Input::ScriptField(const char* label, std::string& moduleName, bool exists)
	{
		bool modified = false;

		Layout::IDGroup(label, [&]() {
			if (!exists && moduleName != "Hazard_NULL") Layout::Text(ICON_FK_TIMES " Not connected");
			if (moduleName == "Hazard_NULL") Layout::Text(ICON_FK_MINUS_SQUARE " Null script");
			if (exists && moduleName != "Hazard_NULL") Layout::Text(ICON_FK_MINUS_SQUARE " Connected");

			Layout::Table(2, false);
			Layout::SetColumnWidth(75);
			Layout::Text(label);
			Layout::TableNext();

			std::string buffer = moduleName;
			Layout::MaxWidth();
			if (Input::InputField(buffer)) {
				moduleName = buffer;
				modified = true;
			}

			Layout::EndTable();
		});
		return modified;
	}
	void Input::Checkbox(const char* label, bool& value)
	{
		ImGui::Checkbox(label, &value);
	}
	void Input::ColorPicker(const char* label, Hazard::Color& color, bool& open)
	{
		if (ImGui::ColorButton(label, Style::ColorAsImVec4(color))) {
			open = true;
		}
		if (open) {
			float c[4] = { color.r, color.g, color.b, color.a };
			ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

			ImGui::Begin(label, &open, flags);

			if (ImGui::ColorPicker4(label, c)) {
				color.r = c[0];
				color.g = c[1];
				color.b = c[2];
				color.a = c[3];
			}
			ImGui::End();
		}
	}
	bool Input::TextureSlot(const char* label, uint32_t& index, uint32_t min, uint32_t max)
	{
		return ImGui::DragInt(label, (int*)&index, 0.1f, min, max);
	}
	bool Input::Slider(const char* label, float& value, float min, float max)
	{
		return ImGui::SliderFloat(label, &value, min, max, "%.3f");
	}
	bool Input::DragInt(const char* label, int& value)
	{
		return ImGui::DragInt(label, &value);
	}
	bool Input::DragUInt(const char* label, uint32_t& value)
	{
		return ImGui::DragInt(label, (int*)&value, 1.0f, 0, 100000);
	}
	bool Input::DragFloat(const char* label, float& value)
	{
		return ImGui::DragFloat(label, &value, 0.025f);
	}
	void Input::PublicField(const char* label, Scripting::PublicField& field)
	{
		using namespace Scripting;

		std::string id("##");
		id.append(label);

		switch (field.type)
		{
			case VarFieldType::Int:
			{
				Layout::Table(2, false);
				Layout::SetColumnWidth(75);
				Layout::Text(label);
				Layout::TableNext();
				Layout::MaxWidth();
				int value = field.GetStoredValue<int>();
				if (Input::DragInt(id.c_str(), value)) {
					field.SetStoredValue(value);
				}
				Layout::EndTable();
				break;
			}
			case VarFieldType::Float: 
			{
				Layout::Table(2, false);
				Layout::SetColumnWidth(75);
				Layout::Text(label);
				Layout::TableNext();
				Layout::MaxWidth();
				float value = field.GetStoredValue<float>();
				if (Input::DragFloat(id.c_str(), value)) {
					field.SetStoredValue(value);
				}
				Layout::EndTable();
				break;
			}
			case VarFieldType::UnsignedInt: 
			{
				Layout::Table(2, false);
				Layout::SetColumnWidth(75);
				Layout::Text(label);
				Layout::TableNext();
				Layout::MaxWidth();
				uint32_t value = field.GetStoredValue<uint32_t>();
				if (Input::DragUInt(id.c_str(), value)) {
					field.SetStoredValue(value);
				}
				Layout::EndTable();
				break;
			}
			case VarFieldType::Vec2:
			{
				glm::vec2 value = field.GetStoredValue<glm::vec2>();
				Layout::IDGroup(label, [&]() {
					if (Input::Vec2(label, value, 0, 75)) {
						field.SetStoredValue(value);
					}
					});
				Layout::NextLine(1);
				break;
			}
			case VarFieldType::Vec3:
			{
				glm::vec3 value = field.GetStoredValue<glm::vec3>();
				Layout::IDGroup(label, [&]() {
					if (Input::Vec3(label, value, 0, 75)) {
						field.SetStoredValue(value);
					}
				});
				Layout::NextLine(1);
				break;
			}
			case VarFieldType::Vec4:
			{
				glm::vec4 value = field.GetStoredValue<glm::vec4>();
				Layout::Text("Vec4 not yet supported");
				break;
			}
			case VarFieldType::String:
			{
				std::string value = field.GetStoredValue<std::string>();
				Layout::Text(label);
				Layout::SameLine(75);
				Layout::MaxWidth();

				if (Input::InputField(value)) {
					field.SetStoredValue(value);
				}
				break;
			}
		}
	}
	void Input::DynamicToggleButton(const char* offLabel, const char* onLabel, const Hazard::Color offColor, const Hazard::Color onColor, bool& modify)
	{
		if (modify) {
			ImGui::PushStyleColor(ImGuiCol_Button, Style::ColorAsImVec4(onColor));
			if (Button(onLabel)) {
				modify = false;
			}
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, Style::ColorAsImVec4(offColor));
			if (Button(offLabel)) {
				modify = true;
			}
		}
		ImGui::PopStyleColor();
	}
	void Input::ToggleButtonColorChange(const char* label, const Hazard::Color offColor, const Hazard::Color onColor, const Hazard::Color textColor, bool& modify, ImVec2 size)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, Style::ColorAsImVec4(textColor));
		if (modify) {
			ImGui::PushStyleColor(ImGuiCol_Button, Style::ColorAsImVec4(onColor));
			if (Button(label, size)) {
				modify = false;
			}
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, Style::ColorAsImVec4(offColor));
			if (Button(label, size)) {
				modify = true;
			}
		}
		ImGui::PopStyleColor(2);
	}
	bool Input::ButtonColorChange(const char* label, const Hazard::Color offColor, const Hazard::Color onColor, const Hazard::Color textColor, const bool state, ImVec2 size)
	{
		bool modified = false;
		ImGui::PushStyleColor(ImGuiCol_Text, Style::ColorAsImVec4(textColor));
		if (state) {
			ImGui::PushStyleColor(ImGuiCol_Button, Style::ColorAsImVec4(onColor));
			if (Button(label, size)) {
				modified = true;
			}
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, Style::ColorAsImVec4(offColor));
			if (Button(label, size)) {
				modified = true;
			}
		}
		ImGui::PopStyleColor(2);
		return modified;
	}
}