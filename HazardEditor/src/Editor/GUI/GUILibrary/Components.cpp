#pragma once
#include <hzreditor.h>
#include "Components.h"
#include "Hazard.h"
#include "VectorControl.h"
#include "Inputs.h"

using namespace Hazard;


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
inline void Draw<SpriteRenderer>(Entity* entity, SpriteRenderer* renderer) {
	Components::DrawComponent<SpriteRenderer>(entity, [](SpriteRenderer* component) {
		static bool tintOpen = false;
		Color newTint = Inputs::LabelledColorPicker("Tint", tintOpen, component->GetTint());
		if (tintOpen)
			component->SetTint(newTint);
		}, [](SpriteRenderer* component, Entity* entity) -> bool {
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
	else if (dynamic_cast<SpriteRenderer*>(component)) {
		Draw<SpriteRenderer>(entity, (SpriteRenderer*)component);
	}
}