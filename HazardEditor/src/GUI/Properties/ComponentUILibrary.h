#pragma once

#include <imgui.h>
#include "GUI/Core/UILibrary.h"

namespace UI
{
	template<typename T>
	static bool ComponentMenuIfExists(Entity& e) {
		if (e.HasComponent<T>())
			return ComponentMenu<T>(e, e.GetComponent<T>());
		return false;
	}
	template<typename T>
	static bool ComponentMenu(Entity& e, T& c) {
		static_assert(false);
	}
	template<>
	static bool ComponentMenu(Entity& e, TagComponent& c) {
		ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		UI::TextField(c.Tag);
		return true;
	}
	template<>
	static bool ComponentMenu(Entity& e, TransformComponent& c) {
		return UI::Treenode("Transform component", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
				ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));

				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text("Translation");
				ImGui::NextColumn();
				UI::Group("Translation", [&]() {
					UI::InputFloat3(c.Translation);
					});
				ImGui::NextColumn();
				ImGui::Text("Rotation");
				ImGui::NextColumn();
				UI::Group("Rotation", [&]() {
					UI::InputFloat3(c.Rotation);
					});
				ImGui::NextColumn();
				ImGui::Text("Scale");
				ImGui::NextColumn();
				UI::Group("Scale", [&]() {
					UI::InputFloat3(c.Scale, 1.0f);
					});
				ImGui::Columns();
			});
	}
}