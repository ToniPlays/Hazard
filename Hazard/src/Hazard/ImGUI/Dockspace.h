#pragma once

#include <imgui.h>
#include "ScopedVar.h"

namespace Hazard::ImUI
{
	class Dockspace 
	{
	public:
		static void BeginDockspace(const char* title, ImGuiDockNodeFlags flags);
		static void EndDockspace(const char* id);
	};
}