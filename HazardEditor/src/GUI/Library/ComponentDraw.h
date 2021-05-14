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
		if (!entity.HasComponent<T>()) return;
		Draw(name, entity, entity.GetComponent<T>());
	}

	template<typename T>
	void Draw(const char* name, Entity entity, T& component) {
		static_assert(false);
	}

	template<>
	inline void Draw(const char* name, Entity entity, TagComponent& component) {
		if (!entity.HasComponent<TagComponent>()) return;
		Layout::ComponentTreenode<TagComponent>(entity, name, [&]() {
			
			Layout::Text("Tag");
			Layout::SameLine(75);
			Layout::MaxWidth();
			Input::InputField(component.m_Tag);
			Layout::NextLine(10);

		}, [&component]() {
			Layout::MenuItem("Reset", [&component]() {
				component.m_Tag = "New entity";
				});
		});
	}

	template<>
	inline void Draw(const char* name, Entity entity, TransformComponent& component) {
		Layout::ComponentTreenode<TransformComponent>(entity, name, [&]() {

			glm::vec3 rot { 
				glm::degrees(component.m_Rotation.x), 
				glm::degrees(component.m_Rotation.y), 
				glm::degrees(component.m_Rotation.z) };

			Layout::IDGroup("Translation", [&]() {
				Input::Vec3("Translation", component.m_Translation, 0, 75);
			});
			Layout::IDGroup("Rotation", [&]() {
				Input::Vec3("Rotation", rot, 0, 75);
			});
			Layout::IDGroup("Scale", [&]() {
				Input::Vec3("Scale", component.m_Scale, 1.0f, 75);
			});

			component.m_Rotation = { glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z) };

		}, [&]() {
			Layout::MenuItem("Reset", [&]() {
				component.m_Translation = { 0, 0, 0 };
				component.m_Rotation = { 0, 0, 0 };
				component.m_Scale = { 1, 1, 1 };
			});
		});
	}
	template<>
	inline void Draw(const char* name, Entity entity, CameraComponent& component) {
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
	inline void Draw(const char* name, Entity entity, SpriteRendererComponent& component) {
		Layout::ComponentTreenode<SpriteRendererComponent>(entity, name, [&]() {

			static bool open = false;

			bool changed = Input::TextureSlot(component.m_Texture.Raw(), [&]() {
				Input::ColorPicker("Sprite tint", component.m_Tint, open);
			});
			if (changed) {
				std::string file = File::OpenFileDialog();
				if (file != "") {
					component.m_Texture = Rendering::RenderUtils::Create<Rendering::Texture2D>(file.c_str());
				}
			}

		}, [&entity]() {

		});
	}
	template<>
	inline void Draw(const char* name, Entity entity, BatchComponent& component) {
		Layout::ComponentTreenode<BatchComponent>(entity, name, [&]() {

			}, [&entity]() {
				
			});
	}

#pragma region Lights 
	template<>
	inline void Draw(const char* name, Entity entity, SkyLightComponent& component) {
		Layout::ComponentTreenode<SkyLightComponent>(entity, name, [&]() {

			Layout::Table(2, false);
			Layout::SetColumnWidth(75);

			Layout::Text("Intensity");
			Layout::TableNext();
			Layout::MaxWidth();
			Input::Slider("##Intensity", component.m_Intensity);

			Layout::TableNext();
			Layout::Text("Tint");
			Layout::TableNext();
			static bool open = false;
			Input::ColorPicker("Sky Light tint", component.m_Tint, open);
			Layout::EndTable();

		}, [&entity]() {
		});
	}
	template<>
	inline void Draw(const char* name, Entity entity, DirectionalLightComponent& component) {
		Layout::ComponentTreenode<DirectionalLightComponent>(entity, name, [&]() {
			
			Layout::Table(2, false);
			Layout::SetColumnWidth(75);

			Layout::Text("Intensity");
			Layout::TableNext();
			Layout::MaxWidth();
			Input::Slider("##Intensity", component.m_Intensity);

			Layout::TableNext();
			Layout::Text("Tint");
			Layout::TableNext();
			static bool open = false;
			Input::ColorPicker("Directional Light tint", component.m_Tint, open);
			Layout::EndTable();

			}, [&entity]() {
			});
	}
	template<>
	inline void Draw(const char* name, Entity entity, PointLightComponent& component) {
		Layout::ComponentTreenode<PointLightComponent>(entity, name, [&]() {

			Layout::Table(2, false);
			Layout::SetColumnWidth(75);

			Layout::Text("Intensity");
			Layout::TableNext();
			Layout::MaxWidth();
			Input::Slider("##Intensity", component.m_Intensity);

			Layout::TableNext();
			Layout::Text("Tint");
			Layout::TableNext();
			static bool open = false;
			Input::ColorPicker("Point Light tint", component.m_Tint, open);
			Layout::TableNext();

			Layout::Text("Radius");
			Layout::TableNext();
			Layout::MaxWidth();
			Input::Slider("##Radius", component.m_Radius);

			Layout::EndTable();
			
			}, [&entity]() {
			});
	}

#pragma endregion

	template<>
	inline void Draw(const char* name, Entity entity, ScriptComponent& component) {
		using namespace Hazard::Scripting;
		Layout::ComponentTreenode<ScriptComponent>(entity, name, [&]() {

			std::string moduleName = component.m_ModuleName;
			bool exists = ScriptCommand::ModuleExists(ScriptType::CSharpScript, moduleName.c_str());

			if (Input::ScriptField("Script", component.m_ModuleName, exists))
			{
				if (ScriptCommand::ModuleExists(ScriptType::CSharpScript, moduleName.c_str())) {
					ScriptCommand::ClearEntity(entity, component);
				}
				if (ScriptCommand::ModuleExists(ScriptType::CSharpScript, component.m_ModuleName.c_str())) {
					ScriptCommand::InitEntity(entity, component);
				}
			}

			if (ScriptCommand::ModuleExists(ScriptType::CSharpScript, moduleName.c_str())) {
				
				for (auto [name, field] : ScriptCommand::GetPublicFields(ScriptType::CSharpScript, entity, moduleName)) {
					Input::PublicField(name, field);
				}
			}

			}, []() {
				Layout::MenuItem("Reload", []() {
					Application::GetModule<ScriptEngineManager>()->ReloadAll();
				});
			});
	}
	template<>
	inline void Draw(const char* name, Entity entity, VisualScriptComponent& component) {
		using namespace Hazard::Scripting;
		Layout::ComponentTreenode<VisualScriptComponent>(entity, name, [&]() {

			}, []() {
				Layout::MenuItem("Reload", []() {
					Application::GetModule<ScriptEngineManager>()->ReloadAll();
				});
			});
	}
	template<>
	inline void Draw(const char* name, Entity entity, MeshComponent& component) {
		Layout::ComponentTreenode<MeshComponent>(entity, name, [&]() {

			Layout::Table(2, false);
			Layout::SetColumnWidth(75);
			if (Input::Button("Mesh")) {
				std::string file = Hazard::File::OpenFileDialog("");
				if (file != "") 
					component.m_Mesh = Hazard::Rendering::MeshFactory::LoadMesh(file);
			}
			Layout::TableNext();
			std::string filename = component.m_Mesh ? component.m_Mesh->GetFile() : "None";
			Layout::Text(filename.c_str());
			Layout::EndTable();
		}, []() {
			
		});
	}
}