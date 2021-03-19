#pragma once
#include <hzreditor.h>
#include "Layout/Layout.h"
#include "Style.h"
#include "Input.h"

using namespace Hazard::ECS;
using namespace WindowLayout;

namespace WindowElement {

	template<typename T>
	void DrawComponent(const char* name, Entity entity) {
		static_assert(false);
	}
	template<>
	inline void DrawComponent<TagComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<TagComponent>()) return;
		auto& component = entity.GetComponent<TagComponent>();
		Layout::ComponentTreenode<TagComponent>(entity, name, [&]() {
			
			Layout::Text("Tag");
			Layout::SameLine(75);
			Layout::MaxWidth();
			Input::InputField(component.tag);
			Layout::NextLine(10);
		}, [&component]() {
			Layout::MenuItem("Reset", [&component]() {
				component.tag = "New entity";
				});
		});	
	}

	template<>
	inline void DrawComponent<TransformComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<TransformComponent>()) return;
		auto& component = entity.GetComponent<TransformComponent>();
		Layout::ComponentTreenode<TransformComponent>(entity, name, [&]() {

			glm::vec3 rot { 
				glm::degrees(component.Rotation.x), 
				glm::degrees(component.Rotation.y), 
				glm::degrees(component.Rotation.z) };

			Layout::IDGroup("Translation", [&]() {
				Input::Vec3("Translation", component.Translation, 0, 100);
			});
			Layout::IDGroup("Rotation", [&]() {
				Input::Vec3("Rotation", rot, 0, 100);
			});
			Layout::IDGroup("Scale", [&]() {
				Input::Vec3("Scale", component.Scale, 1.0f, 100);
			});

			component.Rotation = { glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z) };

		}, [&]() {
			Layout::MenuItem("Reset", [&]() {
				component.Translation = { 0, 0, 0 };
				component.Rotation = { 0, 0, 0 };
				component.Scale = { 1, 1, 1 };
			});
		});
	}
	template<>
	inline void DrawComponent<CameraComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<CameraComponent>()) return;
		auto& component = entity.GetComponent<CameraComponent>();
		Layout::ComponentTreenode<CameraComponent>(entity, name, [&]() {

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)component.GetProjectionType()];

			Layout::Table(2, false);
			Layout::SetColumnWidth(75);
			Layout::Text("Projection");
			Layout::TableNext();
			Layout::MaxWidth();


			if (ImGui::BeginCombo("##Projection", currentProjectionTypeString)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];

					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						component.SetProjection((ECS::Projection)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			Layout::TableNext();
			Layout::Text("Color");
			Layout::TableNext();
			Layout::MaxWidth();

			const char* backgroundTypeString[] = { "Solid", "Skybox" };
			const char* currentBackgroundTypeString = backgroundTypeString[(int)component.GetBackgroundType()];

			if (ImGui::BeginCombo("##backgroundType", currentBackgroundTypeString)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentBackgroundTypeString == backgroundTypeString[i];

					if (ImGui::Selectable(backgroundTypeString[i], isSelected))
					{
						currentBackgroundTypeString = backgroundTypeString[i];
						component.SetBackground((ECS::Background)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			Layout::TableNext();
			Layout::Text("Fov");
			Layout::TableNext();
			Layout::MaxWidth();
			float fov = component.GetFov();
			if (Input::Slider("##FOV", fov, 0.001, 100)) {
				component.SetFov(fov);
			}
			Layout::EndTable();
			glm::vec2 clipping = component.GetClipping();
			if (Input::Vec2("Clipping", clipping, 0, 75)) {
				component.SetZNear(clipping.x);
				component.SetZFar(clipping.y);
			}

			}, [&]() {
				Layout::MenuItem("Reset", [&]() {
					component.SetFov(60);
					component.SetZNear(0.03f);
					component.SetZFar(1000);
				});
			});
	}
	template<>
	inline void DrawComponent<SpriteRendererComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<SpriteRendererComponent>()) return;

		Layout::ComponentTreenode<SpriteRendererComponent>(entity, name, [&entity]() {
			auto& component = entity.GetComponent<SpriteRendererComponent>();


			Layout::Table(2, false);
			Layout::SetColumnWidth(75);
			Layout::Text("Tint");
			Layout::TableNext();
			static bool open = false;
			Input::ColorPicker("Sprite tint", component.tint, open);

			Layout::TableNext();
			Layout::Text("Sprite");
			Layout::TableNext();

			uint32_t value = Rendering::RenderUtils::GetTextureStackIndex(component.texture);

			ImGui::Image((void*)component.texture->GetID(), { 25, 25 });
			Layout::SameLine();
			if (Input::TextureSlot("", value)) {
				*&component.texture = Rendering::RenderUtils::GetFromTextures(value);
			}
			Layout::Text(component.texture->GetFile().c_str());

			Layout::EndTable();

			}, [&entity]() {
			});
	}
	template<>
	inline void DrawComponent<BatchComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<BatchComponent>()) return;
		Layout::ComponentTreenode<BatchComponent>(entity, name, [&entity]() {
			auto& component = entity.GetComponent<BatchComponent>();

			Layout::Table(2, false);
			Layout::SetColumnWidth(75);
			Layout::Text("Size");
			Layout::TableNext();
			Layout::MaxWidth();
			Input::TextureSlot("##Size", component.size, 0, 1000);
			Layout::EndTable();

			}, [&entity]() {
				
			});
	}

#pragma region Lights 
	template<>
	inline void DrawComponent<SkyLightComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<SkyLightComponent>()) return;

		Layout::ComponentTreenode<SkyLightComponent>(entity, name, [&entity]() {
			auto& component = entity.GetComponent<SkyLightComponent>();

			Layout::Table(2, false);
			Layout::SetColumnWidth(75);

			Layout::Text("Intensity");
			Layout::TableNext();
			Layout::MaxWidth();
			Input::Slider("##Intensity", component.intensity);

			Layout::TableNext();
			Layout::Text("Tint");
			Layout::TableNext();
			static bool open = false;
			Input::ColorPicker("Sky Light tint", component.tint, open);
			Layout::EndTable();

		}, [&entity]() {
		});
	}
	template<>
	inline void DrawComponent<DirectionalLightComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<DirectionalLightComponent>()) return;
		Layout::ComponentTreenode<DirectionalLightComponent>(entity, name, [&entity]() {
			auto& component = entity.GetComponent<DirectionalLightComponent>();
			
			Layout::Table(2, false);
			Layout::SetColumnWidth(75);

			Layout::Text("Intensity");
			Layout::TableNext();
			Layout::MaxWidth();
			Input::Slider("##Intensity", component.intensity);

			Layout::TableNext();
			Layout::Text("Tint");
			Layout::TableNext();
			static bool open = false;
			Input::ColorPicker("Directional Light tint", component.tint, open);
			Layout::EndTable();

			}, [&entity]() {
			});
	}
	template<>
	inline void DrawComponent<PointLightComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<PointLightComponent>()) return;
		Layout::ComponentTreenode<PointLightComponent>(entity, name, [&entity]() {
			auto& component = entity.GetComponent<PointLightComponent>();

			Layout::Table(2, false);
			Layout::SetColumnWidth(75);

			Layout::Text("Intensity");
			Layout::TableNext();
			Layout::MaxWidth();
			Input::Slider("##Intensity", component.intensity);

			Layout::TableNext();
			Layout::Text("Tint");
			Layout::TableNext();
			static bool open = false;
			Input::ColorPicker("Point Light tint", component.tint, open);
			Layout::TableNext();

			Layout::Text("Radius");
			Layout::TableNext();
			Layout::MaxWidth();
			Input::Slider("##Radius", component.radius);

			Layout::EndTable();
			
			}, [&entity]() {
			});
	}

#pragma endregion

	template<>
	inline void DrawComponent<ScriptComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<ScriptComponent>()) return;
		auto& component = entity.GetComponent<ScriptComponent>();
		Layout::ComponentTreenode<ScriptComponent>(entity, name, [&]() {

			std::string moduleName = component.moduleName;

			if (Input::ScriptField("Script", component.moduleName, Scripting::ScriptCommand::ModuleExists(moduleName))) {
				if (Scripting::ScriptCommand::ModuleExists(moduleName)) {
					Scripting::ScriptCommand::ShutdownScriptEntity(entity, moduleName);
				}
				if (Scripting::ScriptCommand::ModuleExists(component.moduleName)) {
					Scripting::ScriptCommand::InitScripEntity(entity, component);
				}
			}
			if (Scripting::ScriptCommand::ModuleExists(moduleName)) {
				//Show public fields
				Scripting::EntityInstanceData& instanceData = Scripting::ScriptCommand::GetInstanceData(entity);
				auto& moduleFieldMap = instanceData.moduleFieldMap;
				
				if (moduleFieldMap.find(moduleName) != moduleFieldMap.end()) {
					auto& fields = moduleFieldMap.at(moduleName);

					for (auto& [name, field] : fields) {
						Input::PublicField(name.c_str(), field);
					}
				}
			}
		}, [&]() {
			Layout::MenuItem("Reset", [&]() {
				component.moduleName = "Hazard_NULL";
				});
			Layout::MenuItem("Reload C# assembly", []() {
				Application::GetModule<Scripting::ScriptEngine>().ReloadRuntimeAssembly();
				});
		});
	}
	template<>
	inline void DrawComponent<MeshComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<MeshComponent>()) return;
		auto& component = entity.GetComponent<MeshComponent>();
		Layout::ComponentTreenode<MeshComponent>(entity, name, [&]() {
			Layout::Table(2, false);
			Layout::SetColumnWidth(75);
			if (Input::Button("Mesh")) {
				std::string file = Hazard::Utility::File::OpenFileDialog("");
				if (file != "") 
					component.mesh = Hazard::Rendering::MeshFactory::LoadMesh(file);
			}
			Layout::TableNext();
			std::string filename = component.mesh != nullptr ? component.mesh->GetFile() : "None";
			Layout::Text(filename.c_str());
			Layout::EndTable();
		}, []() {
			
		});
	}
}