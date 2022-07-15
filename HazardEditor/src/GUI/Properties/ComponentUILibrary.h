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
		return UI::Treenode(" " ICON_FK_MAP_MARKER " Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
				ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));


				glm::vec3 rot = { 
					glm::degrees(c.Rotation.x), 
					glm::degrees(c.Rotation.y),
					glm::degrees(c.Rotation.z)
				};


				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, 125.0f);
				UI::InputFloat3("Translation", c.Translation);
				if (UI::InputFloat3("Rotation", rot)) {
					c.Rotation = { glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z) };
				}
				UI::InputFloat3("Scale", c.Scale, 1.0f);
				ImGui::Columns();
			});
	}
	template<>
	static bool ComponentMenu(Entity& e, SpriteRendererComponent& c) {
		return UI::Treenode(" " ICON_FK_PICTURE_O " Sprite renderer", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, 125.0f);

				//Texture slot here

				if (UI::ColorPicker("Tint", "##Tint", c.Tint)) {
					
				}
				ImGui::Columns();
			});
	}
	template<>
	static bool ComponentMenu(Entity& e, CameraComponent& c) {
		return UI::Treenode(" " ICON_FK_VIDEO_CAMERA " Camera", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
				ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 125.0f);

				const char* projectionTypes[] = { "Perspective", "Orthographic" };
				uint32_t selected = (uint32_t)c.GetProjectionType();
				
				//Projection type here
				if (UI::Combo("Projection", "##projection", projectionTypes, 2, selected)) {
					c.SetProjection((Projection)selected);
				}

				glm::vec2 clipping = c.GetClipping();
				if (UI::InputFloat2("Clipping", clipping, 1.0f)) {
					c.SetClipping(clipping);
				}

				ImGui::Columns();
			});
	}
	template<>
	static bool ComponentMenu(Entity& e, ScriptComponent& c) {
		return UI::Treenode(" " ICON_FK_GLOBE " Script", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
			{
				using namespace HazardScript;
				ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());

				std::string oldModule = c.ModuleName;

				auto& scriptEngine = Application::GetModule<ScriptEngine>();
				bool exists = scriptEngine.HasModule(c.ModuleName);
				bool changed = UI::TextField(c.ModuleName, "Script class");
				
				ShiftY(4.0f);

				if (changed) {
					if (scriptEngine.HasModule(oldModule) && c.m_Handle) {
						delete c.m_Handle;
						c.m_Handle = nullptr;
					}
					if (scriptEngine.HasModule(c.ModuleName)) {
						Script& script = scriptEngine.GetScript(c.ModuleName);
						c.m_Handle = script.CreateObject();
					}
				}

				if (c.m_Handle)
				{
					Script& script = c.m_Handle->GetScript();

					ImGui::Columns(2, 0, false);
					ImGui::SetColumnWidth(0, 125.0f);

					for (auto [name, field] : script.GetFields()) {
						
					}
					ImGui::Columns();
				}

				return true;
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
		return UI::Treenode(ICON_FK_CUBE " Mesh", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]()
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