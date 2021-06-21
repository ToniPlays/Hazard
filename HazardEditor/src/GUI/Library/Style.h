#pragma once

#include "Hazard.h"
#include "imgui.h"
#include "imgui_internal.h"

using namespace Hazard;

enum ColorType {
	Primary,
	Secondary,
	Info,
	Debug,
	Warning,
	Error,
	Critical,
	Trace,
	Background,
	Text
};

enum Theme {
	Dark,
	Classic
};

struct ThemeValues
{
	ImVec2 dockPadding;
	ImVec2 framePadding;
};

class Style {
public:
	static void InitTheme(Theme theme);
	static void SetColor(ImGuiCol_ color, Color value);
	static void SetColor(ImGuiCol_ color, ImVec4 value);
	static void SetButtonColors(const char* button, const char* hovered, const char* active);
	static void SetButtonColors(const Color& button, const Color& hovered, const Color& active);
	static void SelectFont(uint8_t index);
	static ImFont* AddFont(const char* file, float size);
	static ImVec4 ColorAsImVec4(const Color& color);

	static ImGuiTreeNodeFlags GetTreeNodeDefaultFlags();
	static ThemeValues& GetStyle() { return s_Values; }
	static Color GetStyleColor(ColorType type);
private:
	static void InitClassic();
	static void InitDarkTheme();
private:
	static Theme s_Theme;
	static ThemeValues s_Values;
};
