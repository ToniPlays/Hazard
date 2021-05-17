#pragma once
#include <hzreditor.h>
#include "Input.h"
#include "Style.h"
#include "Layout/Layout.h"

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
	bool Input::InputField(std::string& text, const char* hint)
	{
		char buffer[512];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), text.c_str());

		if (ImGui::InputTextWithHint("##InputField", hint, buffer, sizeof(buffer)))
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
		bool modified = false;
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
	void Input::ColorPicker(const char* label, Color& color, bool& open)
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
	bool Input::ColoredButton(const char* label, const Hazard::Color color, const Hazard::Color textColor, ImVec2 size)
	{
		bool modified = false;
		ImGui::PushStyleColor(ImGuiCol_Text, Style::ColorAsImVec4(textColor));
		ImGui::PushStyleColor(ImGuiCol_Button, Style::ColorAsImVec4(color));
		if (Button(label, size)) {
			modified = true;
		}
		ImGui::PopStyleColor(2);
		return modified;
	}
	bool Input::ImageButton(uint32_t imageID, ImVec2 size)
	{
		return ImGui::ImageButton((void*)imageID, size);
	}
	bool Input::PublicField(const std::string& name, Scripting::PublicField* field)
	{
		bool modified = false;
		using namespace Hazard::Scripting;

		std::string id("##");
		id.append(name);

		switch (field->GetType())
		{
			case FieldType::Float: 
			{
				Layout::Table(2, false);
				Layout::SetColumnWidth(75);
				Layout::Text(name.c_str());
				Layout::TableNext();
				Layout::MaxWidth();
				float f = field->GetStoredValue<float>();
				modified = DragFloat(id.c_str(), f);
				if (modified) {
					field->SetStoredValue(f);
					if (field->RuntimeAvailable()) 
						field->CopyStoredToRuntimeValue();
				}
				Layout::EndTable();
				break;
			}
			case FieldType::Float2: 
			{
				glm::vec2 f2 = field->GetStoredValue<glm::vec2>();
				modified = Vec2(name.c_str(), f2, 0, 75);
				if (modified) {
					field->SetStoredValue(f2);
					if (field->RuntimeAvailable())
						field->CopyStoredToRuntimeValue();
				}
				Layout::NextLine(1);
				break;
			}
			case FieldType::Float3:
			{
				glm::vec3 f3 = field->GetStoredValue<glm::vec3>();
				modified = Vec3(name.c_str(), f3, 0, 75);
				if (modified) {
					field->SetStoredValue(f3);
					if (field->RuntimeAvailable())
						field->CopyStoredToRuntimeValue();
				}
				Layout::NextLine(1);
				break;
			}
			case FieldType::Int:
			{
				Layout::Table(2, false);
				Layout::SetColumnWidth(75);
				Layout::Text(name.c_str());
				Layout::TableNext();
				Layout::MaxWidth();
				int i = field->GetStoredValue<int>();
				modified = DragInt(id.c_str(), i);
				if (modified) {
					field->SetStoredValue(i);
					if (field->RuntimeAvailable())
						field->CopyStoredToRuntimeValue();
				}
				Layout::EndTable();
				break;
			}
			case FieldType::UInt:
			{
				Layout::Table(2, false);
				Layout::SetColumnWidth(75);
				Layout::Text(name.c_str());
				Layout::TableNext();
				Layout::MaxWidth();
				uint32_t uint = field->GetStoredValue<uint32_t>();
				modified = DragUInt(name.c_str(), uint);
				if (modified) {
					field->SetStoredValue(uint);
					if (field->RuntimeAvailable())
						field->CopyStoredToRuntimeValue();
				}
				Layout::EndTable();
				break;
			}
			default:
			{
				ImGui::Text("Type not recognized");
			}
		}
		Layout::EndTable();
		return modified;
	}
}