
#include "StyleManager.h"

namespace UI {
	void StyleManager::LoadStyle(const Style& style) 
	{
		ImGuiStyle& imStyle = ImGui::GetStyle();

		imStyle.WindowPadding = style.Window.Padding;
		imStyle.WindowBorderSize = style.Window.BorderSize;
		imStyle.WindowTitleAlign = style.Window.TitleAlign;
		imStyle.WindowMenuButtonPosition = style.Window.MenuButtonPosition;
		imStyle.DisplaySafeAreaPadding = style.Window.DisplaySafeArea;
		imStyle.WindowRounding = style.Window.Rounding;

		imStyle.FramePadding = style.Frame.Padding;
		imStyle.FrameRounding = style.Frame.Rounding;
		imStyle.FrameBorderSize = style.Frame.BorderSize;

		imStyle.ScrollbarSize = style.ScrollBar.ScrollbarSize;
		imStyle.ScrollbarRounding = style.ScrollBar.ScrollBarRounding;
		imStyle.GrabRounding = style.ScrollBar.GrabRounding;

		imStyle.ItemInnerSpacing = style.ItemInnerSpacing;
		imStyle.IndentSpacing = style.IndentSpacing;
		imStyle.TabRounding = style.TabRounding;
		
	}
}