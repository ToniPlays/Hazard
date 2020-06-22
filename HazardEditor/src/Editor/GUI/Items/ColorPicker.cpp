#pragma once
#include "hzreditor.h"
#include "ColorPicker.h"

bool ColorPicker::isOpen = false;

void ColorPicker::OnRender() {

	ImGui::Begin("Color picker", &isOpen, ImGuiWindowFlags_NoDocking);

	if (!isOpen) {
		ImGui::End();
		return;
	}

	static float _col[4];
	static float _ref[4] = { color.r, color.g, color.b, color.a };

	ImGui::ColorPicker4("Select color", _col, NULL, _ref);
	color.r = _col[0];
	color.g = _col[1];
	color.b = _col[2];
	color.a = _col[3];

	func(color);
	ImGui::End();
}

