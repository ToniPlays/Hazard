#pragma once
#include <hzreditor.h>
#include "Hierarchy.h"
#include "Inspector.h"

#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif


using namespace Hazard;

Hierarchy::Hierarchy() : Layer("Hierarchy") 
{
	
}
Hierarchy::~Hierarchy() 
{

}
bool Hierarchy::OnEnabled() {

	manager = Hazard::ModuleHandler::GetModule<ECS::SceneManager>();
	context = &manager->GetActiveScene();
	return manager != nullptr;
}
void Hierarchy::Render() {

	if (!Panel::Begin(name, isLayerOpen)) return;
	
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth
		| ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Framed;

	if (ImGui::TreeNodeEx(context->GetFile(), flags)) {
		context->GetRegistry().each([&](auto entityID)
			{
				Hazard::ECS::Entity entity{ entityID, context };
				DrawEntity(entity);
			});
		ImGui::TreePop();
	}
	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
		selectionContext = {};
	}
	if (ImGui::BeginPopupContextWindow(0, 1, false)) {
		if (ImGui::MenuItem("Create empty entity")) {
			context->CreateEntity();
		}
		ImGui::EndPopup();
	}

	Panel::End();
}

void Hierarchy::DrawEntity(Hazard::ECS::Entity entity) {
	

	auto& tag = entity.GetComponent<TagComponent>().Tag;

	ImGuiTreeNodeFlags flags = ((selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
	if (ImGui::IsItemClicked()) {
		selectionContext = entity;
		editor->GetLayer<Inspector>()->SetContext(entity);
	}
	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::MenuItem("Delete entity")) {
			entityDeleted = true;
		}
		ImGui::EndPopup();
	}
	if (opened) {
		//Draw child entities here
		


		ImGui::TreePop();
	}
	if (entityDeleted) {
		context->DestroyEntity(entity);
		if (selectionContext == entity) {
			selectionContext = {};
			editor->GetLayer<Inspector>()->SetContext({});
		}
	}
}
