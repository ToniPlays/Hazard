#pragma once

#include "Hazard.h"

using namespace Hazard;

namespace Style {
	class Style {
	public:
		static void Init();
		static void SetColor(ImGuiCol_ color, Color value);
		static void SetColor(ImGuiCol_ color, const char* value);
		static void SetButtonColors(const char* button, const char* hovered, const char* active);
		static void SetButtonColors(Color button, Color hovered, Color active);
		static void SelectFont(uint8_t index);
		static ImFont* AddFont(const char* file, float size);
		static ImVec4 ColorAsImVec4(Color color);

		static ImGuiTreeNodeFlags GetTreeNodeDefaultFlags();
		static ImGuiStyle& GetStyle() { return ImGui::GetStyle(); }
	};
}