#pragma once

#include <imgui.h>

namespace UI
{
	struct WindowStyle
	{
		ImVec2 Padding = { 2, 2 };
		float BorderSize = 0;
		ImVec2 TitleAlign = { 0.0f, 0.5f };
		ImGuiDir MenuButtonPosition = ImGuiDir_None;
		ImVec2 DisplaySafeArea = { 0, 6 };
		float Rounding = 2;
	};
	struct FrameStyle
	{
		ImVec2 Padding = { 0, 8 };
		float BorderSize = 1;
		float Rounding = 2;
	};
	struct ScrollbarStyle {
		float ScrollbarSize = 16;
		float ScrollBarRounding = 6;
		float GrabRounding = 2;
	};
	struct Style
	{
		WindowStyle Window;
		FrameStyle Frame;
		ScrollbarStyle ScrollBar;

		ImVec2 ItemInnerSpacing = { 6, 4 };

		float IndentSpacing = 16;
		float TabRounding = 2;
	};
}