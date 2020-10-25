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
Color Inputs::ColorPicker(std::string label, bool &open, Color color)
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
Color Inputs::LabelledColorPicker(std::string label, bool& open, Color color) {
	return LabelledColorPicker(label.c_str(), open, color);
}

