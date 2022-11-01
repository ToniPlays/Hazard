#pragma once

#include <imgui.h>
#include "Hazard.h"
#include "Hazard/Scripting/HScript.h"
#include "ScriptFieldUI.h"

#include "Editor/EditorUtils.h"

#include <vector>

namespace UI
{
	constexpr float colWidth = 100.0f;

	using namespace Hazard;

	template<typename T>
	static bool ComponentMenuIfExists(std::vector<Entity>& entities)
	{
		if (AllEntitiesContain<T>(entities))
			return ComponentMenu<T>(entities);
		return false;
	}
	template<typename T>
	static bool ComponentMenu(std::vector<Entity>& entities)
	{
		static_assert(false);
	}
	template<>
	static bool ComponentMenu<TagComponent>(std::vector<Entity>& entities)
	{
		//ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
		//ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		//ImUI::TextField(c.Tag);
		return false;
	}
	template<>
	static bool ComponentMenu<TransformComponent>(std::vector<Entity>& entities)
	{
		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_MAP_MARKER " Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 6.0f));

				auto& firstTc = entities[0].GetComponent<TransformComponent>();

				uint32_t flags = 0;
				glm::vec3 translation = firstTc.GetTranslation();
				glm::vec3 rotation = glm::eulerAngles(firstTc.GetRotation());
				glm::vec3 scale = firstTc.GetScale();

				for (uint32_t i = 1; i < entities.size(); i++)
				{
					auto& tc = entities[i].GetComponent<TransformComponent>();
					auto& t = tc.GetTranslation();
					auto& r = glm::eulerAngles(tc.GetRotation());
					auto& s = tc.GetScale();

					flags |= (t.x != translation.x) ? BIT(0) : 0;
					flags |= (t.y != translation.y) ? BIT(1) : 0;
					flags |= (t.z != translation.z) ? BIT(2) : 0;

					flags |= (r.x != rotation.x) ? BIT(3) : 0;
					flags |= (r.y != rotation.y) ? BIT(4) : 0;
					flags |= (r.z != rotation.z) ? BIT(5) : 0;

					flags |= (s.x != scale.x) ? BIT(6) : 0;
					flags |= (s.y != scale.y) ? BIT(7) : 0;
					flags |= (s.z != scale.z) ? BIT(8) : 0;
				}

				rotation = { glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z) };

				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);
				uint32_t result = ImUI::InputFloat3("Translation", translation, 0.0f, flags);

				if (result != 0)
				{
					for (auto& entity : entities)
					{
						auto& tc = entity.GetComponent<TransformComponent>();
						glm::vec3 position = tc.GetTranslation();

						position.x = (result & BIT(0)) ? translation.x : position.x;
						position.y = (result & BIT(1)) ? translation.y : position.y;
						position.z = (result & BIT(2)) ? translation.z : position.z;

						tc.SetTranslation(position);
					}
				}
				ImUI::ShiftY(3.0f);
				ImGui::Separator();
				ImUI::ShiftY(2.0f);
				result = 0;
				result = ImUI::InputFloat3("Rotation", rotation, 0.0f, flags >> 3);
				if (result != 0)
				{
					for (auto& entity : entities)
					{
						auto& tc = entity.GetComponent<TransformComponent>();
						glm::vec3 rot = glm::eulerAngles(tc.GetRotation());

						rot.x = (result & BIT(0)) ? glm::radians(rotation.x) : rot.x;
						rot.y = (result & BIT(1)) ? glm::radians(rotation.y) : rot.y;
						rot.z = (result & BIT(2)) ? glm::radians(rotation.z) : rot.z;

						glm::quat rotQuat = glm::quat({ rot.x, rot.y, rot.z });
						tc.SetRotation(rotQuat);
					}
				}
				ImUI::ShiftY(3.0f);
				ImGui::Separator();
				ImUI::ShiftY(2.0f);
				result = 0;
				result = ImUI::InputFloat3("Scale", scale, 1.0f, flags >> 6);
				if (result != 0)
				{
					for (auto& entity : entities)
					{
						auto& tc = entity.GetComponent<TransformComponent>();
						glm::vec3 sc = tc.GetScale();

						sc.x = (result & BIT(0)) ? scale.x : sc.x;
						sc.y = (result & BIT(1)) ? scale.y : sc.y;
						sc.z = (result & BIT(2)) ? scale.z : sc.z;

						tc.SetScale(sc);
					}
				}
				ImGui::Columns();

				ImUI::ShiftY(3.0f);
				ImGui::Separator();
				ImUI::ShiftY(2.0f);

			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					for (auto& entity : entities)
					{
						auto& c = entity.GetComponent<TransformComponent>();
						c.SetTranslation({ 0, 0, 0 });
						c.SetRotation({ 0, 0, 0 });
						c.SetScale({ 1, 1, 1 });
					}
					});
			});
		return false;
	}
	template<>
	static bool ComponentMenu<SpriteRendererComponent>(std::vector<Entity>& entities) {
		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_PICTURE_O " Sprite renderer", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {

				auto& firstSr = entities[0].GetComponent<SpriteRendererComponent>();

				uint32_t flags = 0;
				Ref<Texture2DAsset> texture = firstSr.Texture;
				const Color& color = firstSr.Color;

				for (auto& entity : entities)
				{
					auto& sr = entity.GetComponent<SpriteRendererComponent>();
					if (sr.Texture != texture)
					{
						texture = nullptr;
						flags |= BIT(0);
					}

					flags |= (sr.Color.r != color.r) ? BIT(1) : 0;
				}

				std::string path = "None";
				if (texture)
					path = File::GetNameNoExt(AssetManager::GetMetadata(texture->GetHandle()).Path);
				else if (flags & BIT(0)) 
					path = "---";


				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4, 8 });

				ImUI::TextureSlot("Sprite", path, texture, [&]() {
					ImUI::DropTarget<AssetHandle>(AssetType::Image, [&](AssetHandle handle) {
						for (auto& entity : entities)
						{
							auto& c = entity.GetComponent<SpriteRendererComponent>();
							c.Texture = AssetManager::GetAsset<Texture2DAsset>(handle);
						}
						});
					});

				ImGui::PopStyleVar();

				Color c = flags & BIT(1) ? Color::White : color;
				
				if (ImUI::ColorPicker("Tint", "##Tint", c, flags & BIT(1))) 
				{
					for (auto& entity : entities)
					{
						auto& sr = entity.GetComponent<SpriteRendererComponent>();
						sr.Color = c;
					}
				}
				ImGui::Columns();
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {

					for (auto& entity : entities)
					{
						auto& c = entity.GetComponent<SpriteRendererComponent>();
						c.Texture = nullptr;
						c.Color = Color::White;
					}
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				/*
				ImUI::DragSource("Hazard.SpriteRendererComponent", &e.GetUID(), [&]() {
					ImGui::Text("Sprite renderer");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
					*/
			});

		if (removed)
			RemoveComponentFromAll<SpriteRendererComponent>(entities);

		return false;
	}
	template<>
	static bool ComponentMenu<CameraComponent>(std::vector<Entity>& entities)
	{

		bool removed = false;
		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_VIDEO_CAMERA " Camera", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, colWidth);

				const char* projectionTypes[] = { "Perspective", "Orthographic" };
				uint32_t selected = 0; //(uint32_t)c.GetProjectionType();

				//Projection type here
				if (ImUI::Combo("Projection", "##projection", projectionTypes, 2, selected))
				{
					//c.SetProjection((Projection)selected);
				}
				/*
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
				*/

				glm::vec2 clipping;
				if (ImUI::InputFloat2("Clipping", clipping, 1.0f)) {
					//c.SetClipping(clipping);
				}

				ImGui::Columns();
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					//c.SetFov(60.0f);
					//c.SetClipping({ 0.03f, 100.0f });
					//c.SetProjection(Projection::Perspective);
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				/*ImUI::DragSource("Hazard.CameraComponent", &e.GetUID(), [&]() {
					ImGui::Text("Camera component");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
					*/
			});

		if (removed)
			RemoveComponentFromAll<CameraComponent>(entities);

		return false;
	}
	template<>
	static bool ComponentMenu<ScriptComponent>(std::vector<Entity>& entities) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_CODE " Script", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {
				using namespace HazardScript;
				ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

				std::string oldModule = "";

				auto& scriptEngine = Application::GetModule<ScriptEngine>();
				bool exists = scriptEngine.HasModule("");
				bool changed = ImUI::TextFieldWithHint(std::string(""), "Script class");

				ImUI::DropTarget<AssetHandle>(AssetType::Script, [&](AssetHandle handle) {
					Ref<HScript> script = AssetManager::GetAsset<HScript>(handle);

					if (script) {
						//c.ModuleName = script->GetModuleName();
						changed = true;
					}
					});


				if (changed)
				{
					/*
					if (scriptEngine.HasModule(oldModule) && c.m_Handle)
					{
						c.m_Handle = nullptr;
					}
					if (scriptEngine.HasModule(c.ModuleName))
					{
						ScriptMetadata& script = scriptEngine.GetScript(c.ModuleName);
						c.m_Handle = script.CreateObject();
					}
					*/
				}
				/*
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
				*/
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				/*ImUI::DragSource("Hazard.ScriptComponent", &e.GetUID(), [&]() {
					ImGui::Text("Script component");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
					*/
			});

		if (removed)
			RemoveComponentFromAll<ScriptComponent>(entities);

		return false;
	}
	template<>
	static bool ComponentMenu<SkyLightComponent>(std::vector<Entity>& entities) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_GLOBE " Sky light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {

				std::string name = "Empty";
				/*
				if (c.EnvironmentMap)
				{
					AssetHandle handle = c.EnvironmentMap->SourceImage->GetHandle();
					AssetMetadata& data = AssetManager::GetMetadata(handle);
					name = File::GetName(data.Path);
				}
				*/

				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);
				ImGui::Text("Map");
				ImGui::NextColumn();

				ImGui::BeginDisabled();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				ImUI::TextField(name, "##environmentMap");

				ImUI::DropTarget<AssetHandle>(AssetType::Image, [&](AssetHandle handle) {
					Application::Get().SubmitMainThread([&, handle]() {
						Ref<Texture2DAsset> sourceImage = AssetManager::GetAsset<Texture2DAsset>(handle);
						//c.EnvironmentMap = EnvironmentMap::Create(sourceImage);
						});
					});

				ImGui::EndDisabled();

				ImGui::NextColumn();
				//ImUI::SliderFloat("LodLevel", c.LodLevel, 1.0f, 0.0f, 1.0f);
				//ImUI::InputFloat("Intensity", c.Intensity, 1.0f, 0.05f);
				ImGui::Columns();

			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				/*ImUI::DragSource("Hazard.SkyLightComponent", &e.GetUID(), [&]() {
					ImGui::Text("Sky light");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
					*/
			});
		if (removed)
			RemoveComponentFromAll<SkyLightComponent>(entities);

		return false;
	}
	template<>
	static bool ComponentMenu<DirectionalLightComponent>(std::vector<Entity>& entities) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions("Directional light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);

				//if (ImUI::ColorPicker("Color", "##Color", c.LightColor)) {

				//}
				//ImUI::InputFloat("Intensity", c.Intensity, 1.0f, 0.025f);

				ImGui::Columns();
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				/*ImUI::DragSource("Hazard.DirectionalLightComponent", &e.GetUID(), [&]() {
					ImGui::Text("Directional light");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
					*/
			});
		if (removed)
			RemoveComponentFromAll<DirectionalLightComponent>(entities);

		return false;
	}
	template<>
	static bool ComponentMenu<PointLightComponent>(std::vector<Entity>& entities) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions("" ICON_FK_LIGHTBULB_O " Point light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);

				/*if (ImUI::ColorPicker("Color", "##Color", c.LightColor)) {


				ImUI::InputFloat("Intensity", c.Intensity, 1.0f);
				}*/
				ImGui::Columns();
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				/*ImUI::DragSource("Hazard.PointLightComponent", &e.GetUID(), [&]() {
					ImGui::Text("Point light");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
					*/
			});
		if (removed)
			RemoveComponentFromAll<PointLightComponent>(entities);

		return false;
	}
	template<>
	static bool ComponentMenu<MeshComponent>(std::vector<Entity>& entities) {

		bool removed = false;

		bool optionsOpen = ImUI::TreenodeWithOptions(" " ICON_FK_CUBE " Mesh", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]()
			{
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);
				ImGui::Text("Mesh");
				ImGui::NextColumn();

				AssetMetadata data;
				/*if (c.m_MeshHandle)
					data = AssetManager::GetMetadata(c.m_MeshHandle->GetHandle());
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4, 8 });

				if (ImUI::TextFieldWithHint(c.m_MeshHandle ? File::GetNameNoExt(data.Path) : "", "Mesh path"))
				{

				}
				ImGui::PopStyleVar();
					*/

				ImUI::DropTarget<AssetHandle>(AssetType::Mesh, [&](AssetHandle handle) {
					//c.m_MeshHandle = AssetManager::GetAsset<Mesh>(handle);
					});
				ImGui::NextColumn();
				//ImUI::InputFloat("Metalness", c.Metalness, 0.0f, 0.025f, 0.0f, 1.0f);
				//ImUI::InputFloat("Roughness", c.Roughness, 1.0f, 0.025f, 0.0f, 1.0f);

				ImGui::Columns();
			}, [&]() {
				ImUI::MenuItem("Reset", [&]() {
					});
				ImUI::MenuItem("Remove component", [&]() {
					removed = true;
					});
			}, [&]() {
				/*ImUI::DragSource("Hazard.MeshComponent", &e.GetUID(), [&]() {
					ImGui::Text("Mesh");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
					*/
			});
		if (removed)
			RemoveComponentFromAll<MeshComponent>(entities);

		return false;
	}
	template<>
	static bool ComponentMenu<Rigidbody2DComponent>(std::vector<Entity>& entities) {
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
				/*ImUI::DragSource("Hazard.Rigidbody2DComponent", &e.GetUID(), [&]() {
					ImGui::Text("Rigidbody 2D");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
					*/
			});
		if (removed)
			RemoveComponentFromAll<Rigidbody2DComponent>(entities);

		return false;
	}
	template<>
	static bool ComponentMenu<BoxCollider2DComponent>(std::vector<Entity>& entities) {

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
				/*ImUI::DragSource("Hazard.BoxCollider2DComponent", &e.GetUID(), [&]() {
					ImGui::Text("Box collider 2D");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
					*/
			});
		if (removed)
			RemoveComponentFromAll<BoxCollider2DComponent>(entities);

		return false;
	}
	template<>
	static bool ComponentMenu<CircleCollider2DComponent>(std::vector<Entity>& entities) {

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
				/*ImUI::DragSource("Hazard.CircleCollider2DComponent", &e.GetUID(), [&]() {
					ImGui::Text("Circle collider 2D");
					ImGui::Text(std::to_string(e.GetUID()).c_str());
					});
					*/
			});
		if (removed)
			RemoveComponentFromAll<CircleCollider2DComponent>(entities);

		return false;
	}
}
