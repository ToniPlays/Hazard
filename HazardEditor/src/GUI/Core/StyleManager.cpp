
#include "StyleManager.h"

namespace UI {
	void StyleManager::LoadStyle(const Style& style) 
	{
		s_Current = style;
		ImGuiStyle& imStyle = ImGui::GetStyle();
		
		//WindowStyle
		{
			imStyle.WindowPadding = style.Window.Padding;
			imStyle.WindowBorderSize = style.Window.BorderSize;
			imStyle.DisplaySafeAreaPadding = style.Window.DisplaySafeArea;
			imStyle.WindowTitleAlign = style.Window.TitleAlign;
			imStyle.WindowMenuButtonPosition = style.Window.MenuButtonPosition;
			imStyle.WindowRounding = style.Window.Rounding;

			imStyle.Colors[ImGuiCol_TitleBg] = style.Window.TitleBgColor;
			imStyle.Colors[ImGuiCol_TitleBgActive] = style.Window.TitleBgActive;

			imStyle.Colors[ImGuiCol_Header] = style.Window.Header;
			imStyle.Colors[ImGuiCol_HeaderHovered] = style.Window.HeaderHovered;
			imStyle.Colors[ImGuiCol_HeaderActive] = style.Window.HeaderActive;

			imStyle.Colors[ImGuiCol_ResizeGrip] = style.Window.ResizeGrip;
			imStyle.Colors[ImGuiCol_ResizeGripHovered] = style.Window.ResizeGripHovered;
			imStyle.Colors[ImGuiCol_ResizeGripActive] = style.Window.ResizeGripActive;

			imStyle.Colors[ImGuiCol_Text] = style.Window.Text;
			imStyle.Colors[ImGuiCol_TextSelectedBg] = style.Window.TextSelectedBg;
			imStyle.Colors[ImGuiCol_TextDisabled] = style.Window.TextDisabled;

			imStyle.Colors[ImGuiCol_PlotLines] = style.Window.Plotlines;
			imStyle.Colors[ImGuiCol_Border] = style.Window.Border;

			imStyle.Colors[ImGuiCol_CheckMark] = style.Window.Checkmark;
			imStyle.Colors[ImGuiCol_SliderGrab] = style.Window.SliderGrab;
			imStyle.Colors[ImGuiCol_SliderGrabActive] = style.Window.SliderGrabActive;
		}
		//FrameStyle
		{
			imStyle.FramePadding = style.Frame.Padding;
			imStyle.FrameBorderSize = style.Frame.BorderSize;
			imStyle.FrameRounding = style.Frame.Rounding;

			imStyle.Colors[ImGuiCol_FrameBg] = style.Frame.FrameColor;
			imStyle.Colors[ImGuiCol_FrameBgHovered] = style.Frame.FrameHovered;
			imStyle.Colors[ImGuiCol_FrameBgActive] = style.Frame.FrameActive;
		}
		//TabStyle
		{
			imStyle.TabRounding = style.Tab.TabRounding;

			imStyle.Colors[ImGuiCol_Tab] = style.Tab.Tab;
			imStyle.Colors[ImGuiCol_TabActive] = style.Tab.TabActiveFocus;
			imStyle.Colors[ImGuiCol_TabUnfocusedActive] = style.Tab.TabActive;
			imStyle.Colors[ImGuiCol_TabUnfocused] = style.Tab.TabUnfocus;
			imStyle.Colors[ImGuiCol_TabHovered] = style.Tab.TabHovered;
		}
		//DockspaceStyle
		{
			imStyle.Colors[ImGuiCol_DockingPreview] = style.Dockspace.DockingPreview;
		}
		//ScrollbarStyle
		{
			imStyle.ScrollbarSize = style.ScrollBar.ScrollbarSize;
			imStyle.ScrollbarRounding = style.ScrollBar.ScrollBarRounding;
			imStyle.GrabRounding = style.ScrollBar.GrabRounding;

			imStyle.Colors[ImGuiCol_ScrollbarGrab] = style.ScrollBar.ScrollbarGrab;
			imStyle.Colors[ImGuiCol_ScrollbarGrabHovered] = style.ScrollBar.ScrollbarGrabHovered;
			imStyle.Colors[ImGuiCol_ScrollbarGrabActive] = style.ScrollBar.ScrollbarGrabActive;
		}
		//ButtonStyle
		{
			imStyle.Colors[ImGuiCol_Button] = style.Button.Button;
			imStyle.Colors[ImGuiCol_ButtonHovered] = style.Button.ButtonHovered;
			imStyle.Colors[ImGuiCol_ButtonActive] = style.Button.ButtonActive;
		}
		//SeparatorStyle
		{
			imStyle.Colors[ImGuiCol_Separator] = style.Separator.Separator;
			imStyle.Colors[ImGuiCol_SeparatorHovered] = style.Separator.SeparatorHovered;
			imStyle.Colors[ImGuiCol_SeparatorActive] = style.Separator.SeparatorActive;
		}
		//Rest of the style

		imStyle.ItemInnerSpacing = style.ItemInnerSpacing;
		imStyle.IndentSpacing = style.IndentSpacing;

		imStyle.Colors[ImGuiCol_WindowBg] = style.BackgroundColor;
		imStyle.Colors[ImGuiCol_ModalWindowDimBg] = style.ModalBackgroundColor;
		imStyle.Colors[ImGuiCol_MenuBarBg] = style.MenuBarBackground;
		imStyle.Colors[ImGuiCol_NavHighlight] = style.NavHighlight;
		imStyle.Colors[ImGuiCol_ChildBg] = style.ChildBackgroundColor;
	}
}