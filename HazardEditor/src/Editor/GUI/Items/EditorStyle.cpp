#pragma once
#include "EditorStyle.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Editor/Utils/Utility.h"


void EditorStyle::InitStyle() 
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	style->WindowPadding = ImVec2(4, 4);
	style->ItemSpacing = ImVec2(4, 4);
	style->ScrollbarSize = 12;
	style->WindowRounding = 0;
	style->ScrollbarRounding = 0;
	style->GrabMinSize = 12;
	style->WindowBorderSize = 0;
	style->WindowTitleAlign = ImVec2(0, 0.5);
	style->TabRounding = 0;

	//Text
	colors[ImGuiCol_Text] = ColorAsImVec(Hazard::Color::FromHex("#B9B9B9"));
	colors[ImGuiCol_FrameBg] = ColorAsImVec(Hazard::Color::FromHex("#161616"));
	colors[ImGuiCol_PopupBg] = ColorAsImVec(Hazard::Color::FromHex("#000000"));
	colors[ImGuiCol_MenuBarBg] = ColorAsImVec(Hazard::Color::FromHex("#222222"));
	colors[ImGuiCol_ScrollbarGrab] = ColorAsImVec(Hazard::Color::FromHex("#1473c6"));
	colors[ImGuiCol_ScrollbarGrabActive] = ColorAsImVec(Hazard::Color::FromHex("#136DB9"));
	//Title bar
	colors[ImGuiCol_TitleBg] = ColorAsImVec(Hazard::Color::FromHex("#0D4C82FF"));
	colors[ImGuiCol_TitleBgActive] = ColorAsImVec(Hazard::Color::FromHex("#1473C6"));
	colors[ImGuiCol_Tab] = ColorAsImVec(Hazard::Color::FromHex("#222222"));
	colors[ImGuiCol_Separator] = ColorAsImVec(Hazard::Color::FromHex("#1473C6"));
	colors[ImGuiCol_Button] = ColorAsImVec(Hazard::Color::FromHex("#333333FF"));
	colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
}
