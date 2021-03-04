#pragma once
#include <hzreditor.h>
#include "Layout.h"
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
		Layout::ComponentTreenode<TagComponent>(name, [&entity]() {
			auto& component = entity.GetComponent<TagComponent>();
			
			Layout::Text("Tag");
			Layout::SameLine(75);
			Layout::MaxWidth();
			Input::InputField(component.tag);
			Layout::NextLine(10);
		}, []() {

		});	
	}
	template<>
	inline void DrawComponent<TransformComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<TransformComponent>()) return;
		Layout::ComponentTreenode<TransformComponent>(name, [&entity]() {
			auto& component = entity.GetComponent<TransformComponent>();

			glm::vec3 rotation { 
				glm::degrees(component.Rotation.x), 
				glm::degrees(component.Rotation.y), 
				glm::degrees(component.Rotation.z) };

			Layout::IDGroud("Translation", [&]() {
				Input::Vec3("Translation", component.Translation, 0, 100);
			});
			Layout::IDGroud("Rotation", [&]() {
				Input::Vec3("Rotation", rotation, 0, 100);
			});
			Layout::IDGroud("Scale", [&]() {
				Input::Vec3("Scale", component.Scale, 1.0f, 100);
			});

			component.Rotation.x = glm::radians(rotation.x);
			component.Rotation.y = glm::radians(rotation.y);
			component.Rotation.z = glm::radians(rotation.z);

		}, []() {

		});
	}
	template<>
	inline void DrawComponent<CameraComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<CameraComponent>()) return;
		Layout::ComponentTreenode<CameraComponent>(name, [&entity]() {
			auto& component = entity.GetComponent<CameraComponent>();

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)component.GetProjectionType()];

			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
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

			if (Input::Slider("FOV", component.fov, 0.001, 100)) {
				component.RecalculateProjection(component.width, component.height);
			}
			}, []() {

			});
	}
	template<>
	inline void DrawComponent<SpriteRendererComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<SpriteRendererComponent>()) return;

		Layout::ComponentTreenode<SpriteRendererComponent>(name, [&entity]() {
			auto& component = entity.GetComponent<SpriteRendererComponent>();

			Layout::Table(2, false);
			Layout::SetColumnWidth(125);
			Layout::Text("Tint");
			Layout::TableNext();
			Input::ColorPicker("Sprite tint", component.tint);
			Layout::TableNext();
			Layout::Text("Texture");
			Layout::TableNext();

			uint32_t value = Rendering::RenderUtils::GetTextureStackIndex(component.texture);
			ImGui::Image((void*)component.texture->GetID(), { 25, 25 });
			Layout::SameLine();
			Input::TextureSlot("", value);
			Layout::Text(component.texture->GetFile());
			component.texture = Rendering::RenderUtils::GetFromTextures(value);

			Layout::EndTable();

			}, []() {

			});
	}

#pragma region Lights 
	template<>
	inline void DrawComponent<SkyLightComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<SkyLightComponent>()) return;

		Layout::ComponentTreenode<SkyLightComponent>(name, [&entity]() {
			auto& component = entity.GetComponent<SkyLightComponent>();
			Input::Slider("Intensity", component.intensity);
			Input::ColorPicker("Tint", component.tint);
		}, []() {

		});
	}
	template<>
	inline void DrawComponent<DirectionalLightComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<DirectionalLightComponent>()) return;
		Layout::ComponentTreenode<DirectionalLightComponent>(name, [&entity]() {
			auto& component = entity.GetComponent<DirectionalLightComponent>();
			Input::Slider("Intensity", component.intensity);
			Input::ColorPicker("Tint", component.tint);
			}, []() {

			});
	}
	template<>
	inline void DrawComponent<PointLightComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<PointLightComponent>()) return;
		Layout::ComponentTreenode<PointLightComponent>(name, [&entity]() {
			auto& component = entity.GetComponent<PointLightComponent>();

			Input::Slider("Intensity", component.intensity, 0, 10000);
			Input::ColorPicker("Tint", component.tint);

			Input::Vec1("Radius", component.radius, 10, 110);
			}, []() {

			});
	}

#pragma endregion
}