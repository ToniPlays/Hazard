#pragma once
#include "HzrEditor.h"
#include "Hierarchy.h"
#include "Editor/EditorGUI.h"

bool Hierarchy::layerOpen = true;

void Hierarchy::OnRender() const
{
	if (!layerOpen) return;
	ImGui::Begin("Hierarchy", &layerOpen);
	ImGui::Text(("Scene: " + Hazard::Scene::GetCurrent()->name).c_str());

	std::vector<Hazard::GameObject*> objects = Hazard::Scene::GetCurrent()->GetAll();

	if (ImGui::TreeNodeEx("Root")) {
		for (Hazard::GameObject* obj : objects) {

			ListChilds(obj);
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

void Hierarchy::OpenLayer() const
{
	Debug::Log("Opened Hierarchy");
	layerOpen = true;
}

void Hierarchy::CloseLayer() const
{
	Debug::Log("Closed Hierarchy");
	layerOpen = true;
}

void Hierarchy::ListChilds(Hazard::GameObject* obj) const
{
	if (ImGui::TreeNode(obj->name.c_str())) {
		ImGui::TreePop();
	}
}
