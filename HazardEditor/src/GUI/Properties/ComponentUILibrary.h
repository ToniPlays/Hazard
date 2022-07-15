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
		return UI::Treenode(" " ICON_FK_MAP_MARKER " Transform component", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
				ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));

				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, 125.0f);
				UI::InputFloat3("Translation", c.Translation);
				UI::InputFloat3("Rotation", c.Rotation);
				UI::InputFloat3("Scale", c.Scale, 1.0f);
				ImGui::Columns();
			});
	}
	template<>
	static bool ComponentMenu(Entity& e, SpriteRendererComponent& c) {
		return UI::Treenode(" " ICON_FK_PICTURE_O " Sprite renderer component", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{

			});
	}
	template<>
	static bool ComponentMenu(Entity& e, CameraComponent& c) {
		return UI::Treenode(" " ICON_FK_VIDEO_CAMERA " Camera component", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
				
			});
	}
	template<>
	static bool ComponentMenu(Entity& e, SkyLightComponent& c) {
		return UI::Treenode(" " ICON_FK_GLOBE " Sky light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
			});
	}
	template<>
	static bool ComponentMenu(Entity& e, DirectionalLightComponent& c) {
		return UI::Treenode("Directional light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
			});
	}
	template<>
	static bool ComponentMenu(Entity& e, PointLightComponent& c) {
		return UI::Treenode("Point light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
			});
	}
	template<>
	static bool ComponentMenu(Entity& e, MeshComponent& c) {
		return UI::Treenode(ICON_FK_CUBE " Mesh component", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
			});
	}
	template<>
	static bool ComponentMenu(Entity& e, Rigidbody2DComponent& c) {
		return UI::Treenode("Rigidbody 2D", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
			});
	}
	template<>
	static bool ComponentMenu(Entity& e, BoxCollider2DComponent& c) {
		return UI::Treenode(" " ICON_FK_CODEPEN "Box collider 2D", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
			});
	}
	template<>
	static bool ComponentMenu(Entity& e, CircleCollider2DComponent& c) {
		return UI::Treenode(" " ICON_FK_CIRCLE_O "Circle collider 2D", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
			});
	}
}