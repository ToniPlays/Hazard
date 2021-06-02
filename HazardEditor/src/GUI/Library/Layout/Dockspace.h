#pragma once

#include <imgui.h>

namespace WindowLayout {
	class Dockspace {
	public:
		static void Begin(const char* title, ImGuiDockNodeFlags flags);
		static void End(const char* id);

	};
}