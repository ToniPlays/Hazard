#pragma once

#pragma once
#include "Inspector.h"
#include "imgui.h"
#include <sstream>

bool Inspector::layerOpen = true;

Inspector::Inspector()
{
}


void Inspector::Render()
{
	if (!layerOpen) return;

	ImGui::Begin("Inspector", &layerOpen);
	Hazard::Renderer* renderer = Hazard::ModuleHandler::GetModule<Hazard::Renderer>();
	ImGui::Text("GameObjects");

	ImGui::Text("Camera");


	Hazard::GameObject* cam = Hazard::Camera::instance;
	ImGui::DragFloat("CX", &cam->transform.position.x, 0.01f);
	ImGui::DragFloat("CY", &cam->transform.position.y, 0.01f);
	ImGui::DragFloat("CZ", &cam->transform.position.z, 0.01f);

	ImGui::NewLine();
	ImGui::Text("GameObject");

	Hazard::GameObject* go = renderer->GetAll()[0];
	ImGui::DragFloat("GX", &go->transform.position.x, 0.01f);
	ImGui::DragFloat("GY", &go->transform.position.y, 0.01f);
	ImGui::DragFloat("GZ", &go->transform.position.z, 0.01f);

	ImGui::DragFloat("GScale x", &go->transform.scale.x, 0.01f);
	ImGui::DragFloat("GScale y", &go->transform.scale.y, 0.01f);
	ImGui::DragFloat("GScale z", &go->transform.scale.z, 0.01f);

	ImGui::End();
}

