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
		//Text
		colors[ImGuiCol_Text] = Color::asImVec(Color::fromHEX("#B9B9B9"));
		//Title bar
		colors[ImGuiCol_TitleBg] = Color::asImVec(Color::fromHEX("#0c4793"));
		colors[ImGuiCol_TitleBgActive] = Color::asImVec(Color::fromHEX("#1473C6"));

	}
}