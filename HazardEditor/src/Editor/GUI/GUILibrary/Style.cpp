#pragma once
#include <hzreditor.h>
#include "Style.h"

void Style::InitDarkTheme() {

	ImGuiStyle* style = GetStyle();

	style->WindowPadding = ImVec2(4, 4);
	style->FramePadding = ImVec2(4, 2);

	style->WindowTitleAlign = ImVec2(0, 0.5);
	style->WindowMenuButtonPosition = ImGuiDir_Right;

	style->FrameRounding = 1;
	style->WindowRounding = 0;
	style->GrabRounding = 0;
	style->ScrollbarRounding = 0;
	style->TabRounding = 0;


	//Window 
	SetColor(ImGuiCol_WindowBg, "#181818");
	SetColor(ImGuiCol_DockingPreview, "#202020");
	SetColor(ImGuiCol_MenuBarBg, "#141414");
	SetColor(ImGuiCol_NavHighlight, "#53B305");

	//Separators
	SetColor(ImGuiCol_Separator, "#262626");
	SetColor(ImGuiCol_SeparatorHovered, "#212121");
	SetColor(ImGuiCol_SeparatorActive, "#53B305");

	//Headers
	SetColor(ImGuiCol_TitleBg, "#161616");
	SetColor(ImGuiCol_TitleBgActive, "#242424");
	SetColor(ImGuiCol_Header, "#242424");
	SetColor(ImGuiCol_HeaderHovered, "#202020");
	SetColor(ImGuiCol_HeaderActive, "#2E6303");

	//Tabs
	SetColor(ImGuiCol_Tab, "#262626");
	SetColor(ImGuiCol_TabUnfocusedActive, "#262626");
	SetColor(ImGuiCol_TabActive, "#202020");
	SetColor(ImGuiCol_TabUnfocused, "#242424");
	SetColor(ImGuiCol_TabHovered, "#3E6303");


	//Frames
	SetColor(ImGuiCol_FrameBg, "#242424");
	SetColor(ImGuiCol_FrameBgHovered, "#222222");
	SetColor(ImGuiCol_FrameBgActive, "#202020");
	SetColor(ImGuiCol_ResizeGrip, "#262626");
	SetColor(ImGuiCol_ResizeGripHovered, "#2E6303");
	SetColor(ImGuiCol_ResizeGripActive, "#53B305");

	//Text
	SetColor(ImGuiCol_Text, "#E9E9E9");
	SetColor(ImGuiCol_TextDisabled, "#A9A9A9");

	//Lines
	SetColor(ImGuiCol_PlotLines, "#5DC505");
	SetColor(ImGuiCol_Border, "#131313");
	
	//Inputs
	SetColor(ImGuiCol_CheckMark, "#53B305");
	SetColor(ImGuiCol_SliderGrab, "#353535");
	SetColor(ImGuiCol_SliderGrabActive, "#53B305");

	SetColor(ImGuiCol_Button, "#262626");
	SetColor(ImGuiCol_ButtonHovered, "#222222");
	SetColor(ImGuiCol_ButtonActive, "#2E6303");

	SetColor(ImGuiCol_ScrollbarGrab, "#2E6303"); 
	SetColor(ImGuiCol_ScrollbarGrabHovered, "#4A9F04");
	SetColor(ImGuiCol_ScrollbarGrabActive, "#418B04");

}
void Style::SetColor(ImGuiCol_ color, Color value) {
	ImGuiStyle* style = GetStyle();;
	ImVec4* colors = style->Colors;
	
	colors[color] = ColorAsImVec(value);
}
void Style::SetColor(ImGuiCol_ color, const char* value) {
	SetColor(color, Color::FromHex(value));
}

void Style::SetButtonColors(const char* button, const char* hovered, const char* active)
{
	SetButtonColors(Color(button), Color(hovered), Color(active));
}

void Style::SetButtonColors(Color button, Color hovered, Color active)
{
	ImGui::PushStyleColor(ImGuiCol_Button, ColorAsImVec(button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ColorAsImVec(hovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ColorAsImVec(active));
}

void Style::SelectFont(uint8_t index)
{
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[index]);
}
