#pragma once

#include <imgui.h>
#include "ScopedVar.h"
#include "Hazard/Core/Core.h"
#include "HazardRendererCore.h"

namespace Hazard::ImUI
{
	class Dockspace 
	{
	public:
		static void BeginDockspace(const char* title, ImGuiDockNodeFlags flags);
		static void EndDockspace(const char* id);
		static bool CustomTitleBar(HazardRenderer::Window& window, float height);
	};
}