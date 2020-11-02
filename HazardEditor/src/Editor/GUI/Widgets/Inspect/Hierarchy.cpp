#pragma once
#include <hzreditor.h>
#include "Hierarchy.h"
#include "Inspector.h"

using namespace Hazard;

Hierarchy::Hierarchy() : Layer("Hierarchy") 
{
	
}
Hierarchy::~Hierarchy() 
{
}
bool Hierarchy::OnEnabled() {
	return true;
}
void Hierarchy::Render() {

	if (!Panel::Begin(name, isLayerOpen)) return;

	SceneManager* sceneManager = ModuleHandler::GetModule<SceneManager>();
	ImGuiTreeNodeFlags flags = ((entityContext == sceneManager->GetActiveScene()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
	std::hash<std::string> hash;

	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)hash(sceneManager->GetActiveScene()->GetName()), flags, 
		("Scene: " + sceneManager->GetActiveScene()->GetName()).c_str());

	if (ImGui::IsItemClicked()) {
		entityContext = sceneManager->GetActiveScene();
		Editor::GetLayer<Inspector>()->OpenContext((Entity*)entityContext);
	}

	if(opened) {
		DrawEntities(sceneManager->GetActiveScene()->GetChildEntities());
	}	

	if (opened) ImGui::TreePop();

	if (ImGui::BeginPopupContextWindow(0, 1, false)) {
		entityContext = nullptr;
		if (ImGui::MenuItem("New entity")) {
			if(entityContext == nullptr) 
				ModuleHandler::GetModule<SceneManager>()->GetActiveScene()->AddEntity(new Entity("Empty entity"));
		}
		ImGui::EndPopup();
	}
	Panel::End();
}
void Hierarchy::DrawEntities(std::vector<Entity*> entities) {


	for (Entity* entity : entities) {

		ImGuiTreeNodeFlags flags = ((entityContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		std::hash<std::string> hash;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)hash(entity->name), flags, entity->name.c_str());

		if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1)) {
			entityContext = entity;
			Editor::GetLayer<Inspector>()->OpenContext(entityContext);
		}
		
		if (ImGui::BeginPopupContextItem()) {

			if (ImGui::MenuItem("New entity")) 
				entityContext->AddEntity(new Entity("Empty entity"));
			if (ImGui::MenuItem("Duplicate entity")) {}
			if (ImGui::MenuItem("Delete entity")) {
				SceneManager* manager = ModuleHandler::GetModule<SceneManager>();
				manager->GetActiveScene()->RemoveEntity(entityContext);
			}
			ImGui::EndPopup();
		}

		if(opened) {
			DrawEntities(entity->GetChildEntities());
			ImGui::TreePop();
		}
		
	}
}
