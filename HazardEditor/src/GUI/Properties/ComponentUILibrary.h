#pragma once

#include <imgui.h>
#include "Hazard.h"
#include "Hazard/Scripting/HScript.h"
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
		ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		ImUI::TextField(c.Tag);
		return true;
	}
	template<>
	static bool ComponentMenu(Entity& e, TransformComponent& c)
	{
		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_MAP_MARKER " Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 6.0f));

				glm::vec3 rot = {
					glm::degrees(c.Rotation.x),
					glm::degrees(c.Rotation.y),
					glm::degrees(c.Rotation.z)
				};

				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);
				ImUI::InputFloat3("Translation", c.Translation);
				ImUI::ShiftY(3.0f);
				ImGui::Separator();
				ImUI::ShiftY(2.0f);
				if (ImUI::InputFloat3("Rotation", rot)) {
					c.Rotation = { glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z) };
				}
				ImUI::ShiftY(3.0f);
				ImGui::Separator();
				ImUI::ShiftY(2.0f);
				ImUI::InputFloat3("Scale", c.Scale, 1.0f);
				ImGui::Columns();

				ImUI::ShiftY(3.0f);
				ImGui::Separator();
				ImUI::ShiftY(2.0f);

			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
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

		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_PICTURE_O " Sprite renderer", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {

				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4, 8 });
				ImUI::TextureSlot("Sprite", c.Texture, [&]() {
					ImUI::DropTarget<AssetHandle>(AssetType::Image, [&](AssetHandle handle) {
						c.Texture = AssetManager::GetAsset<Texture2DAsset>(handle);
						});
					});
				ImGui::PopStyleVar();

				if (ImUI::ColorPicker("Tint", "##Tint", c.Color)) {

				}
				ImGui::Columns();
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				ImUI::DragSource("Hazard.SpriteRendererComponent", &e.GetUID(), [&]() {
					ImGui::Text("Sprite renderer");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
			});

		if (removed)
			e.RemoveComponent<SpriteRendererComponent>();

		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, CameraComponent& c)
	{

		bool removed = false;
		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_VIDEO_CAMERA " Camera", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, colWidth);

				const char* projectionTypes[] = { "Perspective", "Orthographic" };
				uint32_t selected = (uint32_t)c.GetProjectionType();

				//Projection type here
				if (ImUI::Combo("Projection", "##projection", projectionTypes, 2, selected)) {
					c.SetProjection((Projection)selected);
				}
				if (c.GetProjectionType() == Projection::Perspective) {
					float fov = c.GetFov();

					if (ImUI::InputFloat("Fov", fov, 60.0f)) {
						c.SetFov(fov);
					}
				}
				else
				{
					float size = c.GetSize();

					if (ImUI::InputFloat("Size", size, 60.0f)) {
						c.SetSize(size);
					}
				}

				glm::vec2 clipping = c.GetClipping();
				if (ImUI::InputFloat2("Clipping", clipping, 1.0f)) {
					c.SetClipping(clipping);
				}

				ImGui::Columns();
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					c.SetFov(60.0f);
					c.SetClipping({ 0.03f, 100.0f });
					c.SetProjection(Projection::Perspective);
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				ImUI::DragSource("Hazard.CameraComponent", &e.GetUID(), [&]() {
					ImGui::Text("Camera component");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
			});

		if (removed)
			e.RemoveComponent<CameraComponent>();

		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, ScriptComponent& c) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_CODE " Script", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {
				using namespace HazardScript;
				ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());

				std::string oldModule = c.ModuleName;

				auto& scriptEngine = Application::GetModule<ScriptEngine>();
				bool exists = scriptEngine.HasModule(c.ModuleName);
				bool changed = ImUI::TextFieldWithHint(c.ModuleName, "Script class");

				ImUI::DropTarget<AssetHandle>(AssetType::Script, [&](AssetHandle handle) {
					Ref<HScript> script = AssetManager::GetAsset<HScript>(handle);

					if (script) {
						c.ModuleName = script->GetModuleName();
						changed = true;
					}
					});


				if (changed) 
				{
					if (scriptEngine.HasModule(oldModule) && c.m_Handle)
					{
						c.m_Handle = nullptr;
					}
					if (scriptEngine.HasModule(c.ModuleName)) 
					{
						ScriptMetadata& script = scriptEngine.GetScript(c.ModuleName);
						c.m_Handle = script.CreateObject();
					}
				}

				if (c.m_Handle)
				{
					ScriptMetadata& script = c.m_Handle->GetScript();

					ImGui::Columns(2, 0, false);
					ImGui::SetColumnWidth(0, colWidth);

					auto& world = Application::GetModule<WorldHandler>().GetCurrentWorld();

					for (auto& [name, field] : script.GetFields())
					{
						const char* label = name.c_str();
						Ref<FieldMetadata> f = field;

						ImUI::Group(name.c_str(), [&]() {
							UI::ScriptField(label, f, c.m_Handle, world);
							ImUI::ShiftY(3.0f);
							ImGui::Separator();
							ImUI::ShiftY(2.0f);
							});
					}
					ImGui::Columns();
				}
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				ImUI::DragSource("Hazard.ScriptComponent", &e.GetUID(), [&]() {
					ImGui::Text("Script component");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
			});

		if (removed)
			e.RemoveComponent<ScriptComponent>();

		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, SkyLightComponent& c) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_GLOBE " Sky light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {

				std::string name = "Empty";
				if (c.EnvironmentMap) 
				{
					AssetHandle handle = c.EnvironmentMap->SourceImage->GetHandle();
					AssetMetadata& data = AssetManager::GetMetadata(handle);
					name = File::GetName(data.Path);
				}

				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);
				ImGui::Text("Map");
				ImGui::NextColumn();

				ImGui::BeginDisabled();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
				ImUI::TextField(name, "##environmentMap");

				ImUI::DropTarget<AssetHandle>(AssetType::Image, [&](AssetHandle handle) {
					Application::Get().SubmitMainThread([&, handle]() {
						Ref<Texture2DAsset> sourceImage = AssetManager::GetAsset<Texture2DAsset>(handle);
						c.EnvironmentMap = EnvironmentMap::Create(sourceImage);
						});
					});

				ImGui::EndDisabled();

				ImGui::NextColumn();
				ImUI::InputFloat("Contribution", c.IBLContribution, 1.0f, 0.05f, 0.0f, 1.0f);
				ImUI::InputFloat("Intensity", c.Intensity, 1.0f, 0.05f);
				ImGui::Columns();

			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				ImUI::DragSource("Hazard.SkyLightComponent", &e.GetUID(), [&]() {
					ImGui::Text("Sky light");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
			});
		if (removed)
			e.RemoveComponent<SkyLightComponent>();

		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, DirectionalLightComponent& c) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions("Directional light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);

				if (ImUI::ColorPicker("Color", "##Color", c.LightColor)) {

				}
				ImUI::InputFloat("Intensity", c.Intensity, 1.0f, 0.025f);

				ImGui::Columns();
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				ImUI::DragSource("Hazard.DirectionalLightComponent", &e.GetUID(), [&]() {
					ImGui::Text("Directional light");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
			});
		if (removed)
			e.RemoveComponent<DirectionalLightComponent>();

		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, PointLightComponent& c) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions("" ICON_FK_LIGHTBULB_O " Point light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);

				if (ImUI::ColorPicker("Color", "##Color", c.LightColor)) {

				}

				ImUI::InputFloat("Intensity", c.Intensity, 1.0f);
				ImGui::Columns();
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				ImUI::DragSource("Hazard.PointLightComponent", &e.GetUID(), [&]() {
					ImGui::Text("Point light");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
			});
		if (removed)
			e.RemoveComponent<PointLightComponent>();

		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, MeshComponent& c) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_CUBE " Mesh", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);
				ImGui::Text("Mesh");
				ImGui::NextColumn();

				AssetMetadata data;
				if (c.m_MeshHandle)
					data = AssetManager::GetMetadata(c.m_MeshHandle->GetHandle());

				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4, 8 });

				if (ImUI::TextFieldWithHint(c.m_MeshHandle ? File::GetNameNoExt(data.Path) : "", "Mesh path"))
				{

				}
				ImGui::PopStyleVar();

				ImUI::DropTarget<AssetHandle>(AssetType::Mesh, [&](AssetHandle handle) {
					c.m_MeshHandle = AssetManager::GetAsset<Mesh>(handle);
					});
				ImGui::NextColumn();
				ImUI::InputFloat("Metalness", c.Metalness, 0.0f, 0.025f, 0.0f, 1.0f);
				ImUI::InputFloat("Roughness", c.Roughness, 1.0f, 0.025f, 0.0f, 1.0f);

				ImGui::Columns();
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				ImUI::DragSource("Hazard.MeshComponent", &e.GetUID(), [&]() {
					ImGui::Text("Mesh");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
			});
		if (removed)
			e.RemoveComponent<MeshComponent>();

		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, Rigidbody2DComponent& c) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions("Rigidbody 2D", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{

			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				ImUI::DragSource("Hazard.Rigidbody2DComponent", &e.GetUID(), [&]() {
					ImGui::Text("Rigidbody 2D");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
			});
		if (removed)
			e.RemoveComponent<Rigidbody2DComponent>();

		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, BoxCollider2DComponent& c) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_CODEPEN " Box collider 2D", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				ImUI::DragSource("Hazard.BoxCollider2DComponent", &e.GetUID(), [&]() {
					ImGui::Text("Box collider 2D");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
			});
		if (removed)
			e.RemoveComponent<BoxCollider2DComponent>();

		return false;
	}
	template<>
	static bool ComponentMenu(Entity& e, CircleCollider2DComponent& c) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_CIRCLE_O " Circle collider 2D", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				ImUI::DragSource("Hazard.CircleCollider2DComponent", &e.GetUID(), [&]() {
					ImGui::Text("Circle collider 2D");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
			});
		if (removed)
			e.RemoveComponent<CircleCollider2DComponent>();

		return false;
	}
}
