
#include "Style.h"

Theme Style::s_Theme;
ThemeValues Style::s_Values;

void Style::InitTheme(Theme theme) {
	s_Theme = theme;

	switch (theme)
	{
	case Dark:		InitDarkTheme(); break;
	case Classic:	InitClassic(); break;
	}
}
void Style::SetColor(ImGuiCol_ color, const Color& value)
{
	SetColor(color, ColorAsImVec4(value));
}
void Style::SetColor(ImGuiCol_ color, const ImVec4& value)
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[color] = value;
}
void Style::SetButtonColors(const char* button, const char* hovered, const char* active)
{
	SetButtonColors(Color(button), Color(hovered), Color(active));
}
void Style::SetButtonColors(const Color& button, const Color& hovered, const Color& active)
{
	ImGui::PushStyleColor(ImGuiCol_Button, ColorAsImVec4(button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ColorAsImVec4(hovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ColorAsImVec4(active));
}
void Style::SelectFont(uint16_t index)
{
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[index]);
}
ImFont* Style::AddFont(const char* file, float size)
{
	return ImGui::GetIO().Fonts->AddFontFromFileTTF(file, size);
}
ImVec4 Style::ColorAsImVec4(const Color& color)
{
	return { color.r, color.g, color.b, color.a };
}
ImU32 Style::ColorWithMultiplier(ImVec4 color, float multiplier)
{
	const ImVec4& colRow = color;
	float hue, sat, val;
	ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
	return ImColor::HSV(hue, sat, std::min(val * multiplier, 1.0f));
}
ImVec4 Style::AssetTypeColor(Hazard::AssetType type)
{
	switch (type)
	{
	case AssetType::Undefined:			return ColorAsImVec4(GetStyleColor(ColorType::Warning));
	case AssetType::AudioClip:			return ColorAsImVec4(GetStyleColor(ColorType::Debug));
	case AssetType::World:				return ColorAsImVec4(GetStyleColor(ColorType::Primary));
	case AssetType::Image:				return ColorAsImVec4(GetStyleColor(ColorType::Critical));
	case AssetType::Mesh:				return ColorAsImVec4(GetStyleColor(ColorType::Info));
	case AssetType::PhysicsMaterial:	return ColorAsImVec4(GetStyleColor(ColorType::Primary));
	case AssetType::EnvironmentMap:		return ColorAsImVec4(Style::GetStyleColor(ColorType::Trace));
    default:                            return ColorAsImVec4(GetStyleColor(ColorType::Text));
	}
}
ImGuiTreeNodeFlags Style::GetTreeNodeDefaultFlags()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed;
	return flags;
}
Color Style::GetStyleColor(ColorType type)
{
	switch (type)
	{
	case ColorType::Primary:		return Color("#53B305");
	case ColorType::Secondary:		return Color("#282828");
	case ColorType::Info:			return Color("#1AE61A");
	case ColorType::Debug:			return Color("#0080ff");
	case ColorType::Warning:		return Color("#FFE600");
	case ColorType::Error:			return Color("#FF331A");
	case ColorType::Critical:		return Color("#FF0000");
	case ColorType::Trace:			return Color::White;
	case ColorType::Background:		return Color("#181818");
	case ColorType::Text:			return Color("#B9B9B9");
	}
	return Color::Black;
}
ImU32 Style::GetStyleColor32(ColorType type)
{
	return ImGui::ColorConvertFloat4ToU32(ColorAsImVec4(GetStyleColor(type)));
}
void Style::InitClassic()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding = ImVec2(4, 4);
	style.FramePadding = ImVec2(6, 2);

	style.WindowTitleAlign = ImVec2(0, 0.5);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.DisplaySafeAreaPadding = { 0, 6 };

	style.IndentSpacing = 8;
	style.ScrollbarSize = 16;

	style.FrameRounding = 2;
	style.WindowRounding = 0;
	style.GrabRounding = 0;
	style.ScrollbarRounding = 0;
	style.TabRounding = 2;

	s_Values.dockPadding = { 6, 2 };
	s_Values.framePadding = { 6, 2 };

	//Window 
	SetColor(ImGuiCol_WindowBg, GetStyleColor(ColorType::Background));
	SetColor(ImGuiCol_DockingPreview, "#202020");
	SetColor(ImGuiCol_MenuBarBg, "#141414");
	SetColor(ImGuiCol_NavHighlight, GetStyleColor(ColorType::Primary));

	//Separators
	SetColor(ImGuiCol_Separator, "#262626");
	SetColor(ImGuiCol_SeparatorHovered, "#212121");
	SetColor(ImGuiCol_SeparatorActive, GetStyleColor(ColorType::Primary));

	//Headers
	SetColor(ImGuiCol_TitleBg, "#161616");
	SetColor(ImGuiCol_TitleBgActive, GetStyleColor(ColorType::Secondary));
	SetColor(ImGuiCol_Header, GetStyleColor(ColorType::Secondary));
	SetColor(ImGuiCol_HeaderHovered, "#202020");
	SetColor(ImGuiCol_HeaderActive, "#2E6303");

	//Tabs
	SetColor(ImGuiCol_Tab, "#262626");
	SetColor(ImGuiCol_TabUnfocusedActive, "#262626");
	SetColor(ImGuiCol_TabActive, "#202020");
	SetColor(ImGuiCol_TabUnfocused, GetStyleColor(ColorType::Secondary));
	SetColor(ImGuiCol_TabHovered, "#3E6303");


	//Frames
	SetColor(ImGuiCol_FrameBg, GetStyleColor(ColorType::Secondary));
	SetColor(ImGuiCol_FrameBgHovered, "#222222");
	SetColor(ImGuiCol_FrameBgActive, "#202020");
	SetColor(ImGuiCol_ResizeGrip, GetStyleColor(ColorType::Secondary));
	SetColor(ImGuiCol_ResizeGripHovered, "#2E6303");
	SetColor(ImGuiCol_ResizeGripActive, GetStyleColor(ColorType::Primary));

	//Text
	SetColor(ImGuiCol_Text, GetStyleColor(ColorType::Text));
	SetColor(ImGuiCol_TextDisabled, "#A9A9A9");

	//Lines
	SetColor(ImGuiCol_PlotLines, "#5DC505");
	SetColor(ImGuiCol_Border, "#131313");

	//Inputs
	SetColor(ImGuiCol_CheckMark, GetStyleColor(ColorType::Primary));
	SetColor(ImGuiCol_SliderGrab, "#353535");
	SetColor(ImGuiCol_SliderGrabActive, GetStyleColor(ColorType::Primary));

	SetColor(ImGuiCol_Button, "#262626");
	SetColor(ImGuiCol_ButtonHovered, "#222222");
	SetColor(ImGuiCol_ButtonActive, "#2E6303");

	SetColor(ImGuiCol_ScrollbarGrab, "#2E6303");
	SetColor(ImGuiCol_ScrollbarGrabHovered, "#4A9F04");
	SetColor(ImGuiCol_ScrollbarGrabActive, "#418B04");
}
void Style::InitDarkTheme()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding = { 2, 2 };
	style.WindowBorderSize = 0;
	style.DisplaySafeAreaPadding = { 0, 6 };

	style.FrameRounding = 2;
	style.WindowRounding = 2;
	style.FrameBorderSize = 1;
	style.ScrollbarRounding = 6;
	style.IndentSpacing = 16;
	style.GrabRounding = 2;
	style.ItemInnerSpacing = { 6, 4 };
	style.TabRounding = 2;
	style.WindowMenuButtonPosition = ImGuiDir_None;

	s_Values.dockPadding = { 16, 6 };
	s_Values.framePadding = { 0, 8 };

	SetColor(ImGuiCol_MenuBarBg, "#181816");
	SetColor(ImGuiCol_Text, "#B9B9B9");
	SetColor(ImGuiCol_TextSelectedBg, "#5DC5054B");

	SetColor(ImGuiCol_WindowBg, "#222222");
	SetColor(ImGuiCol_ScrollbarBg, "#222222");
	SetColor(ImGuiCol_TitleBgActive, "#181816");

	SetColor(ImGuiCol_Separator, "#181816");
	SetColor(ImGuiCol_SeparatorHovered, "#5DC50570");
	SetColor(ImGuiCol_SeparatorActive, GetStyleColor(ColorType::Primary));

	SetColor(ImGuiCol_Border, "#303030");
	SetColor(ImGuiCol_CheckMark, "#5DC505");
	SetColor(ImGuiCol_NavHighlight, "#5DC50590");

	SetColor(ImGuiCol_FrameBg, "#0D0D0B");
	SetColor(ImGuiCol_FrameBgHovered, "#0B0B09");
	SetColor(ImGuiCol_FrameBgActive, "#0B0B09");

	SetColor(ImGuiCol_Header, "#323234");
	SetColor(ImGuiCol_HeaderHovered, "#2e2e30");

	SetColor(ImGuiCol_Tab, "#181816");
	SetColor(ImGuiCol_TabActive, "#222222");
	SetColor(ImGuiCol_TabHovered, "#5DC50594");
	SetColor(ImGuiCol_TabUnfocused, "#161616");
	SetColor(ImGuiCol_TabUnfocusedActive, "#222222");

	SetColor(ImGuiCol_DockingPreview, "#5DC5052A");

	SetColor(ImGuiCol_Button, "#222222");
	SetColor(ImGuiCol_ButtonHovered, "#181818");
	SetColor(ImGuiCol_ButtonActive, "#222222");

	SetColor(ImGuiCol_SliderGrab, "#303030");

	SetColor(ImGuiCol_ResizeGrip, "#181816");
	SetColor(ImGuiCol_ResizeGripHovered, "#5DC50570");
	SetColor(ImGuiCol_ResizeGripActive, GetStyleColor(ColorType::Primary));

	SetColor(ImGuiCol_DragDropTarget, "#5DC50594");
}
