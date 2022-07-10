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
}