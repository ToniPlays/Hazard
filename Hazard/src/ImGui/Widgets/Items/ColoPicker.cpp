#pragma once
#include <hzrpch.h>
#include "ColorPicker.h"
#include "imgui.h"

namespace Hazard {

	bool ColorPicker::isOpen = false;

	void ColorPicker::OnRender() {

		ImGui::Begin("Color picker", &isOpen);
		static float _col[4];
		ImGui::ColorPicker4("Select color", _col);
		color.r = _col[0];
		color.g = _col[1];
		color.b = _col[2];
		color.a = _col[3];

		getterLayer->OnGetColor(color);

		ImGui::End();
	}
}