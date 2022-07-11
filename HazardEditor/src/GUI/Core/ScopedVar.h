#pragma once

#include <imgui.h>

namespace UI {
	template<typename Value>
	class ScopedStyleVar {
	public:
		ScopedStyleVar(ImGuiStyleVar var, Value value) {
			ImGui::PushStyleVar(var, value);
		}
		~ScopedStyleVar() {
			ImGui::PopStyleVar();
		}
	};
	class ScopedStyleColor {
	public:
		ScopedStyleColor(ImGuiStyleVar var, ImVec4 value) {
			ImGui::PushStyleColor(var, value);
		}
		~ScopedStyleColor() {
			ImGui::PopStyleColor();
		}
	};
}