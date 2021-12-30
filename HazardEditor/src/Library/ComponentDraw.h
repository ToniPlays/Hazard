#pragma once

#include "Layout/Layout.h"
#include "Style.h"
#include "Input.h"
#include "Library/DragDropUtils.h"
#include "Core/SceneRuntimeHandler.h"

using namespace Hazard::ECS;
using namespace WindowLayout;


namespace WindowElement {

	template<typename T>
	void Draw(const char* name, Entity entity, T& component) {
		assert(false);
	}

	template<>
	inline void Draw(const char* name, Entity entity, TagComponent& component) {
		Layout::ComponentTreenode<TagComponent>(entity, name, [&]() {

			Layout::Text("Tag");
			Layout::SameLine(75);
			Layout::MaxWidth();
			Input::InputField(component.Tag);
			Layout::NextLine(10);

			}, [&component]() {
				Layout::MenuItem("Reset", [&component]() {
					component.Tag = "New entity";
					});
			});
	}

	template<>
	inline void Draw(const char* name, Entity entity, TransformComponent& component) {
		Layout::ComponentTreenode<TransformComponent>(entity, name, [&]() {

			glm::vec3 rot{
				glm::degrees(component.Rotation.x),
				glm::degrees(component.Rotation.y),
				glm::degrees(component.Rotation.z)
			};

			Layout::IDGroup("Translation", [&]() {
				Input::Vec3("Translation", component.Translation, 0, 75);
				});
			Layout::IDGroup("Rotation", [&]() {
				Input::Vec3("Rotation", rot, 0, 75);
				});
			Layout::IDGroup("Scale", [&]() {
				Input::Vec3("Scale", component.Scale, 1.0f, 75);
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

			bool changed = Input::TextureSlot(component.Texture, [&]() {
				Input::ColorPicker("Sprite tint", component.Tint, open);
				}, [&]() {
					DragDropUtils::DragTarget("Image", [&](const ImGuiPayload* payload) {
						AssetHandle handle = *(AssetHandle*)payload->Data;
						component.Texture = AssetManager::GetAsset<Rendering::Texture2D>(handle);
						});
				});
			if (changed) {
				std::string file = File::OpenFileDialog("");
				if (!file.empty())
				{
					using namespace Hazard::Rendering;
					Texture2DCreateInfo createInfo;
					createInfo.FilePath = file;
				}
			}

			}, [&entity]() {

			});
	}
	template<>
	inline void Draw(const char* name, Entity entity, BatchComponent& component) {
		Layout::ComponentTreenode<BatchComponent>(entity, name, [&]() {
			Input::DragUInt("Size", component.Size, 0.0f, 1000.0f);

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
			Input::Slider("##Intensity", component.Intensity);

			Layout::TableNext();
			Layout::Text("Tint");
			Layout::TableNext();
			static bool open = false;
			Input::ColorPicker("Sky Light tint", component.Tint, open);
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
			Input::Slider("##Intensity", component.Intensity);

			Layout::TableNext();
			Layout::Text("Tint");
			Layout::TableNext();
			static bool open = false;
			Input::ColorPicker("Directional Light tint", component.Tint, open);
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
			Input::Slider("##Intensity", component.Intensity);

			Layout::TableNext();
			Layout::Text("Tint");
			Layout::TableNext();
			static bool open = false;
			Input::ColorPicker("Point Light tint", component.Tint, open);
			Layout::TableNext();

			Layout::Text("Radius");
			Layout::TableNext();
			Layout::MaxWidth();
			Input::Slider("##Radius", component.Radius);

			Layout::EndTable();

			}, [&entity]() {
			});
	}

#pragma endregion

	template<>
	inline void Draw(const char* name, Entity entity, ScriptComponent& component)
	{
		using namespace Hazard::Scripting;
		Layout::ComponentTreenode<ScriptComponent>(entity, name, [&]() {

			std::string moduleName = component.ModuleName;
			bool exists = ScriptCommand::ModuleExists(ScriptType::CSharpScript, moduleName.c_str());
			bool changed = Input::ModuleField("Script", component.ModuleName, exists);

			DragDropUtils::DragTarget("Script", [&](const ImGuiPayload* payload) {
				moduleName = (const char*)payload->Data;

				component.ModuleName = File::GetNameNoExt(moduleName);
				changed = true;
				});

			Layout::Separator(2.0f);
			if (changed) {
				if (ScriptCommand::ModuleExists(ScriptType::CSharpScript, moduleName.c_str())) {
					ScriptCommand::ClearEntity((uint32_t)entity, component);
				}
				if (ScriptCommand::ModuleExists(ScriptType::CSharpScript, component.ModuleName.c_str())) {
					ScriptCommand::InitEntity((uint32_t)entity, component);
				}
			}

			if (ScriptCommand::ModuleExists(ScriptType::CSharpScript, moduleName.c_str())) {
				bool runtime = Runtime::SceneRuntimeHandler::IsSceneRunning();

				for (auto& [index, field] : ScriptCommand::GetFields(ScriptType::CSharpScript, (uint32_t)entity, moduleName))
				{
					Input::ScriptField(field, runtime);
				}
			}
			}, []() {
				Layout::MenuItem("Reload", []() {
					Application::GetModule<ScriptEngineManager>().ReloadAll();
					});
			});
	}
	template<>
	inline void Draw(const char* name, Entity entity, VisualScriptComponent& component) {
		using namespace Hazard::Scripting;
		Layout::ComponentTreenode<VisualScriptComponent>(entity, name, [&]() {

			}, []() {
				Layout::MenuItem("Reload", []() {
					Application::GetModule<ScriptEngineManager>().ReloadAll();
					});
			});
	}
	template<>
	inline void Draw(const char* name, Entity entity, AudioSourceComponent& component) {
		using namespace Hazard::Scripting;

		Layout::ComponentTreenode<AudioSourceComponent>(entity, name, [&]() {
			Layout::Text("Source");
			Layout::SameLine(75);
			Layout::MaxWidth();
			Input::InputField(component.SourceFile);

			DragDropUtils::DragTarget("AudioClip", [&](const ImGuiPayload* payload) {
				AssetHandle handle = *(AssetHandle*)payload->Data;
				if (handle != INVALID_ASSET_HANDLE) {
					component.SourceFile = AssetManager::GetMetadata(handle).Path.string();
					component.Source.SetSourceBuffer(AssetManager::GetAsset<Audio::AudioBufferData>(handle));
				}
				});

			Layout::NextLine(5);

			Layout::Text("Gain");
			Layout::SameLine(75);
			Layout::MaxWidth();
			if (Input::DragFloat("##Gain", component.Gain, 0.005f, 0, 1.0f))
				component.Source.SetGain(component.Gain);

			Layout::Text("Pitch");
			Layout::SameLine(75);
			Layout::MaxWidth();
			if (Input::DragFloat("##Pitch", component.Pitch, 0.005f, 0, 0))
				component.Source.SetPitch(component.Pitch);

			Layout::Text("Looping");
			Layout::SameLine(200);
			Layout::MaxWidth();
			if (Input::Checkbox("##Looping", component.Looping))
				component.Source.SetLoop(component.Looping);

			Layout::Text("Spatial");
			Layout::SameLine(200);
			Layout::MaxWidth();
			if (Input::Checkbox("##spatial", component.Spatial))
				component.Source.SetSpatial(component.Spatial);

			Layout::Text("Play on Create");
			Layout::SameLine(200);
			Layout::MaxWidth();
			Input::Checkbox("##playOnCreate", component.PlayOnCreate);

			}, []() {

			});
	}
	template<>
	inline void Draw(const char* name, Entity entity, MeshComponent& component) {
		Layout::ComponentTreenode<MeshComponent>(entity, name, [&]() {

			Layout::Text("Mesh");
			Layout::SameLine(75);
			Layout::MaxWidth();
			Ref<Rendering::Mesh> mesh = component.m_Mesh;

			std::string file = "None";
			bool changed = Input::InputField(file);

			DragDropUtils::DragTarget("Mesh", [&](const ImGuiPayload* payload) {
				file = (const char*)payload->Data;
				changed = true;
				});

			if (changed) {
				if (!File::Exists(file)) return;
				HZR_CORE_INFO("New mesh file {0}", file);
			}
			}, []() {

			});
	}
	template<>
	inline void Draw(const char* name, Entity entity, Rigidbody2DComponent& component) {
		Layout::ComponentTreenode<Rigidbody2DComponent>(entity, name, [&]() {

			Layout::Table(2, false);
			Layout::SetColumnWidth(75);
			Layout::Text("Body type");
			Layout::TableNext();

			const char* text[] = { "Static", "Kinematic", "Dynamic" };
			const char* currentText;
			currentText = text[(int)component.Type];

			if (ImGui::BeginCombo("##bodyType", currentText)) {
				for (int i = 0; i < 3; i++) {

					bool isSelected = currentText == text[i];
					if (ImGui::Selectable(text[i], isSelected))
					{
						currentText = text[i];
						component.Type = (Physics::BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			Layout::TableNext();
			Layout::Text("Fixed rotation");
			Layout::TableNext();
			Layout::MaxWidth();
			Input::Checkbox("##", component.FixedRotation);

			Layout::EndTable();
			}, []() {

			});
	}

	template<>
	inline void Draw(const char* name, Entity entity, BoxCollider2DComponent& component) {
		Layout::ComponentTreenode<BoxCollider2DComponent>(entity, name, [&]() {
			Layout::IDGroup("#offset", [&]() {
				Input::Vec2("Offset", component.Offset, 1.0f, 75);
				});
			Layout::IDGroup("#Size", [&]() {
				Input::Vec2("Size", component.Size, 1.0f, 75);
				});
			Layout::NextLine(5);

			Layout::Text("Sensor");
			Layout::SameLine(75);
			Layout::MaxWidth();
			Input::Checkbox("##sensor", component.IsSensor);
			Layout::NextLine(5);

			Layout::Text("Density");
			Layout::SameLine(75);
			Layout::MaxWidth();
			Input::DragFloat("##Density", component.Density);
			},
			[]() {
			});
	}
	template<>
	inline void Draw(const char* name, Entity entity, CircleCollider2DComponent& component) {
		Layout::ComponentTreenode<CircleCollider2DComponent>(entity, name, [&]() {
			Layout::IDGroup("#offset", [&]() {
				Input::Vec2("Offset", component.Offset, 1.0f, 75);
				});

			Input::DragFloat("Radius", component.Radius);
			Layout::NextLine(5);

			Layout::Text("Sensor");
			Layout::SameLine(75);
			Layout::MaxWidth();
			Input::Checkbox("##sensor", component.IsSensor);
			Layout::NextLine(5);

			Layout::Text("Density");
			Layout::SameLine(75);
			Layout::MaxWidth();
			Input::DragFloat("##Density", component.Density);
			},
			[]() {
			});
	}
	template<typename T>
	void DrawComponent(const char* name, Entity entity) {
		if (entity.HasComponent<T>())
			Draw(name, entity, entity.GetComponent<T>());
	}

}