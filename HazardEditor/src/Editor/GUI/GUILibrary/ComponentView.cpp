#pragma once

#include <hzreditor.h>
#include "ComponentView.h"
#include "VectorControl.h"
#include "Editor/GUI/GUILibrary/Inputs.h"
#include "Editor/Core/ComponentRegister.h"


void ComponentView::DrawComponents(Entity* entity) {

	for (Component* component : entity->GetComponents()) {
		if (dynamic_cast<Transform*>(component)) {
			DrawComponent<Transform>(entity, [](Transform* component) {

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
		else if (dynamic_cast<SpriteRenderer*>(component)) {
			DrawComponent<SpriteRenderer>(entity, [](SpriteRenderer* component) {

				static bool tintOpen = false;
				Color newTint = Inputs::LabelledColorPicker("Tint", tintOpen, component->GetTint());
				if(tintOpen)
					component->SetTint(newTint);

				}, [](SpriteRenderer* component, Entity* entity) -> bool {
					if (ImGui::MenuItem("Remove component")) {
						entity->RemoveComponent(component);
						return true;
					}
					return false;
				});
		}
	}
}
template<typename T, typename Draw, typename Context>
inline void ComponentView::DrawComponent(Entity* entity, Draw fn, Context ctx)
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
		if(!modified)
			fn((T*)component);
		ImGui::TreePop();
	}
}
void ComponentView::ViewAll(Entity* entity) {

	std::vector<ComponentInfo> components = ModuleHandler::GetModule<ComponentRegister>()->GetAll();

	for (ComponentInfo info : components) {
		if (ImGui::MenuItem(info.path.c_str())) {
			info.fn(entity);
		}
	}
}