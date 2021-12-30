#pragma once

#include <imgui_internal.h>
#include <imgui.h>
#include <hzrpch.h>
#include "Hazard/Rendering/Texture.h"

namespace WindowLayout {
	class Dockspace {
	public:
		static void Begin(const char* title, ImGuiDockNodeFlags flags);
		static void End(const char* id);

	};
}
namespace ImGui
{

    bool TreeNodeWithIcon(Ref<Hazard::Rendering::Texture2D> icon, ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end, ImColor iconTint = IM_COL32_WHITE);
    bool TreeNodeWithIcon(Ref<Hazard::Rendering::Texture2D> icon, const void* ptr_id, ImGuiTreeNodeFlags flags, ImColor iconTint, const char* fmt, ...);
    bool TreeNodeWithIcon(Ref<Hazard::Rendering::Texture2D> icon, const char* label, ImGuiTreeNodeFlags flags, ImColor iconTint = IM_COL32_WHITE);

} // namespace ImGui