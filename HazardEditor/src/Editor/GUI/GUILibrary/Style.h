#pragma once

#include <imgui.h>
#include <imgui_internal.h>

class Style {

public:
	static void InitDarkTheme();

	static void SetColor(ImGuiCol_ color, Color value);
	static void SetColor(ImGuiCol_ color, const char* value);
	static void SetButtonColors(const char* button, const char* hovered, const char* active);
	static void SetButtonColors(Color button, Color hovered, Color active);
	static void SelectFont(uint8_t index);
	static ImGuiStyle* GetStyle() { return &ImGui::GetStyle(); }
};