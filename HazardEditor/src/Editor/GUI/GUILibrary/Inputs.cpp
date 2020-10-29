#pragma once

#include <hzreditor.h>
#include "Inputs.h"

void Inputs::ResettableDragButton(const char* label, float& value, float resetValue, ImVec2 size, uint32_t buttonFont, uint32_t dragFont)
{
	Style::SelectFont(buttonFont);

	if (ImGui::Button(label, size)) {
		value = resetValue;
	}

	ImGui::PopFont();
	Style::SelectFont(dragFont);

	ImGui::SameLine();
	std::stringstream ss;
	ss << "##" << label;
	ImGui::DragFloat(ss.str().c_str(), &value, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopFont();
}

void Inputs::InputField(std::string& text)
{
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy_s(buffer, sizeof(buffer), text.c_str());

	if (ImGui::InputText("##InputField", buffer, sizeof(buffer)))
	{
		text = buffer;
	}
}

void Inputs::Texture(Hazard::Texture* texture, Hazard::Vector2<int> size)
{
	ImGui::Image((void*)texture->GetID(), {(float)size.x, (float)size.y }, ImVec2(0, 1), ImVec2(1, 0));
}

Color Inputs::ColorPicker(const char* label, bool &open, Color color)
{
	float c[4] = { color.r, color.g, color.b, color.a };
	ImGui::Begin(label, &open);

	if (ImGui::ColorPicker4(label, c)) {
		color.r = c[0];
		color.g = c[1];
		color.b = c[2];
		color.a = c[3];
	}
	ImGui::End();
	return color;
}
Color Inputs::ColorPicker(std::string& label, bool &open, Color color)
{
	return ColorPicker(label.c_str(), open, color);
}

Color Inputs::LabelledColorPicker(const char* label, bool &open, Color color)
{
	ImGui::Columns(2);
	ImGui::Text(label);
	ImGui::NextColumn();
	if (ImGui::ColorButton(label, ColorAsImVec(color)))
		open = true;
	if (open)
		color = Inputs::ColorPicker(label, open, color);
	ImGui::Columns(1);
	return color;
}
Color Inputs::LabelledColorPicker(std::string& label, bool& open, Color color) {
	return LabelledColorPicker(label.c_str(), open, color);
}

bool Inputs::ConfirmDialog(const char* label, const char* description)
{
	return false;
}

bool Inputs::ConfirmDialog(std::string& label, std::string& description)
{
	return ConfirmDialog(label.c_str(), description.c_str());
}

bool Inputs::Combo(const char* label, std::vector<std::string> values, std::string& selected)
{
	BeginColumnRow(label, 2);
	Inputs::MaxWidth();
	if (ImGui::BeginCombo("##Label", selected.c_str())) {
		
		for (std::string value : values) {
			bool isSelected = value == selected;
			if (ImGui::Selectable(value.c_str(), isSelected)) {
				ImGui::EndCombo();
				Inputs::EndColumnRow();
				selected = value;
				return true;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		
		ImGui::EndCombo();
	}
	Inputs::EndColumnRow();
	return false;
}

bool Inputs::Combo(std::string label, std::vector<std::string> values, std::string& selected)
{
	return Combo(label, values, selected);
}

void Inputs::BeginColumnRow(const char* label, uint8_t columns)
{
	ImGui::PushID(label);
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 80);
	ImGui::Text(label);

	ImGui::NextColumn();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
	ImGui::PushMultiItemsWidths(columns, ImGui::CalcItemWidth());
}

void Inputs::BeginColumnRow(std::string label, uint8_t columns)
{
	BeginColumnRow(label.c_str(), columns);
}

void Inputs::EndColumnRow(uint8_t columns)
{
	ImGui::PopStyleVar();
	ImGui::Columns(columns);
	ImGui::PopID();
}

void Inputs::MaxWidth()
{
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
}

