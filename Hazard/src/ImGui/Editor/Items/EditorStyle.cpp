#pragma once
#include <hzrpch.h>
#include "imgui.h"
#include "EditorStyle.h"
#include "Utils/Color.h"

namespace Hazard {

	void EditorStyle::InitStyle() {
		HZR_CORE_INFO("Initializing ImGui style");
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		style->WindowPadding = ImVec2(4, 4);
		style->ItemSpacing = ImVec2(4, 4);
		style->ScrollbarSize = 12;
		style->WindowRounding = 0;
		style->ScrollbarRounding = 0;
		style->GrabMinSize = 12;
		style->WindowBorderSize = 0;
		style->WindowTitleAlign = ImVec2(1, 0.5);

		//Text
		colors[ImGuiCol_Text] = Color::asImVec(Color::fromHEX("#B9B9B9"));
		//Title bar
		colors[ImGuiCol_TitleBg] = Color::asImVec(Color::fromHEX("#0c4793"));
		colors[ImGuiCol_TitleBgActive] = Color::asImVec(Color::fromHEX("#1473C6"));

		colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
	}
}