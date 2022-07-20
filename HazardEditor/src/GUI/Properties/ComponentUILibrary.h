#pragma once

#include <imgui.h>
#include "Hazard.h"
#include "GUI/Core/UILibrary.h"
#include "ScriptFieldUI.h"

namespace UI
{
	constexpr float colWidth = 100.0f;

	template<typename T>
	static bool ComponentMenuIfExists(Entity& e) {
		if (e.HasComponent<T>()) {
			bool ret = ComponentMenu<T>(e, e.GetComponent<T>());
			//const Style& style = StyleManager::GetCurrent();
			//Separator({ ImGui::GetContentRegionAvailWidth(), 2.0f }, style.Frame.FrameColor);
			return ret;
		}
		return false;
	}
	template<typename T>
	static bool ComponentMenu(Entity& e, T& c) {
		static_assert(false);
	}
	template<>
	static bool ComponentMenu(Entity& e, TagComponent& c) {
		ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		UI::TextField(c.Tag);
		return true;
	}
	template<>
	static bool ComponentMenu(Entity& e, TransformComponent& c) {
		bool optionsOpen = UI::TreenodeWithOptions(" " ICON_FK_MAP_MARKER " Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 6.0f));

				glm::vec3 rot = {
					glm::degrees(c.Rotation.x),
					glm::degrees(c.Rotation.y),
					glm::degrees(c.Rotation.z)
				};

				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);
				UI::InputFloat3("Translation", c.Translation);
				ShiftY(3.0f);
				ImGui::Separator();
				ShiftY(2.0f);
				if (UI::InputFloat3("Rotation", rot)) {
					c.Rotation = { glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z) };
				}
				ShiftY(3.0f);
				ImGui::Separator();
				ShiftY(2.0f);
				UI::InputFloat3("Scale", c.Scale, 1.0f);
				ImGui::Columns();

				ShiftY(3.0f);
				ImGui::Separator();
				ShiftY(2.0f);

					}, [&]() {
						MenuItem("Reset", [&]() {
							c.Translation = { 0, 0, 0 };
							c.Rotation = { 0, 0, 0 };
							c.Scale = { 1, 1, 1 };
							});
					});

				return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, SpriteRendererComponent& c) {
		bool removed = false;

		bool optionsOpen = UI::TreenodeWithOptions(" " ICON_FK_PICTURE_O " Sprite renderer", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);

				//Texture slot here

				if (UI::ColorPicker("Tint", "##Tint", c.Tint)) {

				}
				ImGui::Columns();
			}, [&]() {
				MenuItem("Remove component", [&]() {
					removed = true;
					});
			});

		if (removed) {
			e.RemoveComponent<SpriteRendererComponent>();
		}

		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, CameraComponent& c)
	{
		bool optionsOpen = UI::TreenodeWithOptions(" " ICON_FK_VIDEO_CAMERA " Camera", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, colWidth);

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
			}, [&]() {
				MenuItem("Reset", [&]() {
					c.SetFov(60.0f);
					c.SetClipping({ 0.03f, 100.0f });
					c.SetProjection(Projection::Perspective);
					});
			});


		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, ScriptComponent& c) {
		bool optionsOpen = UI::Treenode(" " ICON_FK_CODE " Script", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				using namespace HazardScript;
				ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());

				std::string oldModule = c.ModuleName;

				auto& scriptEngine = Application::GetModule<ScriptEngine>();
				bool exists = scriptEngine.HasModule(c.ModuleName);
				bool changed = UI::TextField(c.ModuleName, "Script class");

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
					ImGui::SetColumnWidth(0, colWidth);

					for (auto& [name, field] : script.GetFields())
					{
						const char* label = name.c_str();
						auto& f = field;

						Group(name.c_str(), [&]() {
							UI::ScriptField(label, f, *c.m_Handle);
							ShiftY(3.0f);
							ImGui::Separator();
							ShiftY(2.0f);
							});
					}
					ImGui::Columns();
				}

				return true;
			});
		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, SkyLightComponent& c) {
		bool optionsOpen = UI::Treenode(" " ICON_FK_GLOBE " Sky light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{

			});
		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, DirectionalLightComponent& c) {
		bool optionsOpen = UI::Treenode("Directional light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);

				if (UI::ColorPicker("Color", "##Color", c.LightColor)) {

				}
				InputFloat("Intensity", c.Intensity, 1.0f);

				ImGui::Columns();
			});
		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, PointLightComponent& c) {
		bool optionsOpen = UI::Treenode("" ICON_FK_LIGHTBULB_O " Point light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);

				//Texture slot here
				if (UI::ColorPicker("Color", "##Color", c.LightColor)) {

				}

				InputFloat("Intensity", c.Intensity, 1.0f);
				ImGui::Columns();
			});
		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, MeshComponent& c) {
		bool optionsOpen = UI::Treenode(" " ICON_FK_CUBE " Mesh", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);
				ImGui::Text("Mesh");
				ImGui::NextColumn();

				if (ImGui::Button("Import Mesh")) {
					std::string path = File::OpenFileDialog("");
					AssetHandle handle = Hazard::AssetManager::ImportAsset(path);
					c.m_MeshHandle = Hazard::AssetManager::GetAsset<Hazard::Mesh>(handle);
					HZR_WARN("Loading mesh file {0}", path);
				}

				ImGui::Columns();
			});
		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, Rigidbody2DComponent& c) {
		bool optionsOpen = UI::Treenode("Rigidbody 2D", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
			});
		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, BoxCollider2DComponent& c) {
		bool optionsOpen = UI::Treenode(" " ICON_FK_CODEPEN " Box collider 2D", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
			});
		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, CircleCollider2DComponent& c) {
		bool optionsOpen = UI::Treenode(" " ICON_FK_CIRCLE_O " Circle collider 2D", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
			});
		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, BatchComponent& c)
	{
		bool optionsOpen = UI::Treenode("Batch renderer", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);

				//Texture slot here

				if (UI::ColorPicker("Tint", "##Tint", c.Tint)) {

				}
				float size = c.Size;
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
				UI::InputFloat("Size", size, 1.0f);
				c.Size = size;
				ImGui::Columns();
			});
		return false;
	}
}
