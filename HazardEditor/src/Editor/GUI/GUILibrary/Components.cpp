#pragma once
#include <hzreditor.h>
#include "Components.h"
#include "Hazard.h"
#include "VectorControl.h"
#include "Inputs.h"

using namespace Hazard;

std::string CameraTypeToString(CameraType type) {
	return type == CameraType::Perspective ? "Perspective" : "Orthographic";
}
CameraType StringToType(std::string type) {
	return type == "Perspective" ? CameraType::Perspective : CameraType::Orthographic;
}


template<typename T, typename Draw, typename Context>
inline void Components::DrawComponent(Entity* entity, Draw fn, Context ctx)
{
	ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
	float lineHeight = GImGui->Font->FontSize + Style::GetStyle()->FramePadding.y * 2.0f;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
	std::hash<std::string> hash;

	ImGui::Separator();
	if (!entity->HasComponent<T>()) return;

	T* component = entity->GetComponent<T>();
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)hash(component->GetName()), flags,
		(component->GetName().c_str()));


	bool modified = false;

	ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
	if (ImGui::Button("+", ImVec2(lineHeight, lineHeight))) {
		ImGui::OpenPopup("ContextMenu");
	}
	if (ImGui::BeginPopup("ContextMenu")) {
		modified = ctx((T*)component, entity);
		ImGui::EndPopup();
	}
	if (opened) {
		if (!modified)
			fn((T*)component);
		ImGui::TreePop();
	}
}

template<typename T>
void Draw(Entity* entity, T* component) {};

template<>
inline void Draw<Transform>(Entity* entity, Transform* transform) {
	Components::DrawComponent<Transform>(entity, [](Transform* component) {

		VectorControl::DrawVec3("Position", component->position);
		VectorControl::DrawVec3("Rotation", component->rotation);
		VectorControl::DrawVec3("Scale", component->scale, 1.0f);

	}, [](Transform* component, Entity* entity) -> bool {
		if (ImGui::MenuItem("Remove component")) {
			entity->RemoveComponent(component);
			return true;
		}
		return false;
	});
}
template<>
inline void Draw<CameraComponent>(Entity* entity, CameraComponent* transform) {
	Components::DrawComponent<CameraComponent>(entity, [](CameraComponent* component) {

		std::string type = CameraTypeToString(component->GetType());
		
		if (Inputs::Combo("Projection", { "Perspective", "Orthographic" }, type)) {
			component->SetType(StringToType(type));
		}

		static bool colorOpen = false;
		Color newColor = Inputs::LabelledColorPicker("Tint", colorOpen, component->clearColor);

		if (colorOpen)
			component->clearColor = newColor;

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
		ImVec2 buttonSize = { lineHeight + 15.0f, lineHeight };

		Inputs::BeginColumnRow(component->GetType() == CameraType::Perspective ? "Fov" : "Size", 2);
		Inputs::MaxWidth();

		if (component->GetType() == CameraType::Perspective)
			ImGui::SliderFloat("##Fov", &component->FovSize, 0, 180);
		else ImGui::DragFloat("##Size", &component->FovSize, 0.1f, 0.001f, 250);
		ImGui::PopItemWidth();
		Inputs::EndColumnRow(1);

		Inputs::BeginColumnRow("Clipping", 2);
		Style::SetButtonColors("#5DC505", "#4A9F04", "#418B04");
		Inputs::ResettableDragButton("Near", component->clipping.x,
			component->GetType() == CameraType::Perspective ? 0.03f : -1.0f, buttonSize);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		Style::SetButtonColors("#1651F3", "#0B41D5", "#0935AE");
		Inputs::ResettableDragButton("Far", component->clipping.y, 
			component->GetType() == CameraType::Perspective ? 1000 : 1.0, buttonSize);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();
		
		Inputs::EndColumnRow(1);

		}, [](CameraComponent* component, Entity* entity) -> bool {
			if (ImGui::MenuItem("Remove component")) {
				entity->RemoveComponent(component);
				return true;
			}
			return false;
		});
}
template<>
inline void Draw<SpriteRenderer>(Entity* entity, SpriteRenderer* renderer) {
	Components::DrawComponent<SpriteRenderer>(entity, [](SpriteRenderer* component) {
		static bool tintOpen = false;
		Color newTint = Inputs::LabelledColorPicker("Tint", tintOpen, component->GetTint());

		if (tintOpen)
			component->SetTint(newTint);
		ImGui::InputInt("TextureIndex", &component->textureIndex);
		component->textureIndex = Hazard::Math::Clamp(component->textureIndex, 0, 10);

		}, [](SpriteRenderer* component, Entity* entity) -> bool {
			if (ImGui::MenuItem("Remove component")) {
				entity->RemoveComponent(component);
				return true;
			}
			return false;
		});
}
template<>
inline void Draw<BatchRendererComponent>(Entity* entity, BatchRendererComponent* renderer) {
	Components::DrawComponent<BatchRendererComponent>(entity, [](BatchRendererComponent* component) {

		VectorControl::DravVec1("Size", component->size, 1);

		}, [](BatchRendererComponent* component, Entity* entity) -> bool {
			if (ImGui::MenuItem("Remove component")) {
				entity->RemoveComponent(component);
				return true;
			}
			return false;
		});
}

void Components::DrawComponent(Entity* entity, Component* component) {
	if (dynamic_cast<Transform*>(component)) {
		Draw<Transform>(entity, (Transform*)component);
	}
	else if (dynamic_cast<CameraComponent*>(component)) {
		Draw<CameraComponent>(entity, (CameraComponent*)component);
	}
	else if (dynamic_cast<SpriteRenderer*>(component)) {
		Draw<SpriteRenderer>(entity, (SpriteRenderer*)component);
	}
	else if (dynamic_cast<BatchRendererComponent*>(component)) {
		Draw<BatchRendererComponent>(entity, (BatchRendererComponent*)component);
	}
}