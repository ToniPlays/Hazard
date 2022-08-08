#pragma once

#include "Color.h"
#include <imgui.h>

namespace Hazard::ImUI
{
	inline static ImVec4 ColorToImVec(const Color& color) 
	{
		return { color.r, color.g, color.b, color.a };
	}

	struct WindowStyle
	{
		ImVec2 Padding = { 2, 2 };
		float BorderSize = 0.0f;
		float PopupBorderSize = 0.0f;

		ImVec2 TitleAlign = { 0.5f, 0.5f };
		ImGuiDir MenuButtonPosition = ImGuiDir_None;
		ImVec2 DisplaySafeArea = { 0, 6 };
		float Rounding = 2;

		ImVec4 TitleBgColor = ColorToImVec("#181816");
		ImVec4 TitleBgActive = ColorToImVec("#181816");
		ImVec4 Header = ColorToImVec("#323234");
		ImVec4 HeaderHovered = ColorToImVec("#2E2E30");
		ImVec4 HeaderActive = ColorToImVec("#2E6303");

		ImVec4 ResizeGrip = ColorToImVec("#181816");
		ImVec4 ResizeGripHovered = ColorToImVec("#5DC50570");
		ImVec4 ResizeGripActive = ColorToImVec("#53B305");

		ImVec4 Text = ColorToImVec("#B9B9B9");
		ImVec4 TextSelectedBg = ColorToImVec("#5DC5054B");
		ImVec4 TextDisabled = ColorToImVec("#909090");

		ImVec4 Plotlines = ColorToImVec("#53B305");
		ImVec4 Border = ColorToImVec("#303030");

		ImVec4 Checkmark = ColorToImVec("#5DC505");
		ImVec4 SliderGrab = ColorToImVec("#303030");
		ImVec4 SliderGrabActive = ColorToImVec("#505050");

	};
	struct FrameStyle
	{
		ImVec2 Padding = { 0, 8 };
		float BorderSize = 0;
		float Rounding = 2;

		ImVec4 FrameColor = ColorToImVec("#0D0D0B");
		ImVec4 FrameHovered = ColorToImVec("#0B0B09");
		ImVec4 FrameActive = ColorToImVec("#0B0B09");
	};
	struct TabStyle 
	{
		float TabRounding = 2;

		ImVec4 Tab = ColorToImVec("#202020");
		ImVec4 TabActiveFocus = ColorToImVec("#323230");
		ImVec4 TabActive = ColorToImVec("#242422");
		ImVec4 TabUnfocus = ColorToImVec("#161614");
		ImVec4 TabHovered = ColorToImVec("#5DC50594");
	};

	struct DockspaceStyle 
	{
		ImVec4 DockingPreview = ColorToImVec("#5DC5052A");
	};
	
	struct ScrollbarStyle 
	{
		float ScrollbarSize = 16;
		float ScrollBarRounding = 6;
		float GrabRounding = 2;

		ImVec4 ScrollbarGrab = ColorToImVec("#2E6303");
		ImVec4 ScrollbarGrabHovered = ColorToImVec("#4A9F04");
		ImVec4 ScrollbarGrabActive = ColorToImVec("#418B04");
	};
	struct ButtonStyle 
	{
		ImVec4 Button = ColorToImVec("#222222");
		ImVec4 ButtonHovered = ColorToImVec("#181818");
		ImVec4 ButtonActive = ColorToImVec("#222222");
	};
	struct SeparatorStyle 
	{
		ImVec4 Separator = ColorToImVec("#181816");
		ImVec4 SeparatorHovered = ColorToImVec("#5DC50570");
		ImVec4 SeparatorActive = ColorToImVec("#5DC505");
	};
	struct StylePrimaryColors {
		ImVec4 AxisX = ColorToImVec("#DB3721");
		ImVec4 AxisY = ColorToImVec("#53B305");
		ImVec4 AxisZ = ColorToImVec("#1651F3");

		ImVec4 Warning = ColorToImVec("#FFE600");

	};
	struct Style
	{
		WindowStyle Window;
		FrameStyle Frame;
		TabStyle Tab;
		DockspaceStyle Dockspace;
		ScrollbarStyle ScrollBar;
		ButtonStyle Button;
		SeparatorStyle Separator;
		StylePrimaryColors Colors;

		ImVec2 ItemInnerSpacing = { 6, 4 };

		float IndentSpacing = 16;

		ImVec4 BackgroundColor = ColorToImVec("#202020");
		ImVec4 ModalBackgroundColor = ColorToImVec("#0A0A0A80");
		ImVec4 ChildBackgroundColor = ColorToImVec("#262626");
		ImVec4 MenuBarBackground = ColorToImVec("#181816");
		ImVec4 NavHighlight = ColorToImVec("#5DC50590");
	};
}