#pragma once

#pragma once
#include "Inspector.h"
#include "Editor/Core/Analytics/Debug.h"
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
	Hazard::GlobalRenderer* renderer = Hazard::ModuleHandler::GetModule<Hazard::GlobalRenderer>();
	ImGui::Text("GameObjects");

	Hazard::GameObject* cam = Hazard::Camera::instance;
	//Position

	ImGui::Text("Position");
	ImGui::SetNextItemWidth(70);
	ImGui::DragFloat("PX", &cam->transform.position.x, 0.01f);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(70);
	ImGui::DragFloat("PY", &cam->transform.position.y, 0.01f);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(70);
	ImGui::DragFloat("PZ", &cam->transform.position.z, 0.01f);
	//Rotation
	ImGui::Text("Rotation");
	ImGui::SetNextItemWidth(70);
	ImGui::DragFloat("RX", &cam->transform.rotation.x, 0.1f);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(70);
	ImGui::DragFloat("RY", &cam->transform.rotation.y, 0.1f);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(70);
	ImGui::DragFloat("RZ", &cam->transform.rotation.z, 0.1f);
	//Scale
	ImGui::Text("Scale");
	ImGui::SetNextItemWidth(70);
	ImGui::DragFloat("SX", &cam->transform.scale.x, 0.01f);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(70);
	ImGui::DragFloat("SY", &cam->transform.scale.y, 0.01f);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(70);
	ImGui::DragFloat("SZ", &cam->transform.scale.z, 0.01f);


	ImGui::End();
}

