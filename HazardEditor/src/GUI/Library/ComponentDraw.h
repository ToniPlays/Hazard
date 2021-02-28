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
			Layout::Text("Camera component todo");
			}, []() {

			});
	}
	template<>
	inline void DrawComponent<SpriteRendererComponent>(const char* name, Entity entity) {
		if (!entity.HasComponent<SpriteRendererComponent>()) return;

		Layout::ComponentTreenode<SpriteRendererComponent>(name, [&entity]() {
			auto& component = entity.GetComponent<SpriteRendererComponent>();
			Layout::Table(2, false);
			Layout::Text("Tint");
			Layout::TableNext();
			Input::ColorPicker("Tint", component.tint);
			Layout::EndTable();

			}, []() {

			});
	}
}