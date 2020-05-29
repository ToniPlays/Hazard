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
	Hazard::Renderer* renderer = Hazard::ModuleHandler::GetModule<Hazard::Renderer>();
	ImGui::Text("GameObjects");

	ImGui::Text("Camera");


	Hazard::Camera* cam = Hazard::Camera::instance;
	ImGui::DragFloat("CX", &cam->transform.position.x, 0.01f);
	ImGui::DragFloat("CY", &cam->transform.position.y, 0.01f);
	ImGui::DragFloat("CZ", &cam->transform.position.z, 0.01f);
	
	static float size = cam->GetSize();

	ImGui::DragFloat("Size", &size, 0.01f);

	size = Hazard::Math::Clamp(size, 0.0f, Hazard::Math::MaxValue<float>());
	cam->SetSize(size);

	if (ImGui::Button("Print matrix")) {
		Hazard::Matrix4 matrix = Hazard::Matrix4::TransformMatrix(cam->GetTransform());
		std::cout << matrix.ToString() << std::endl;
	}


	ImGui::NewLine();
	ImGui::Text("GameObject");

	Hazard::GameObject* go = renderer->GetAll()[0];
	ImGui::DragFloat("GX", &go->transform.position.x, 0.01f);
	ImGui::DragFloat("GY", &go->transform.position.y, 0.01f);
	ImGui::DragFloat("GZ", &go->transform.position.z, 0.01f);

	ImGui::DragFloat("GScale x", &go->transform.scale.x, 0.01f);
	ImGui::DragFloat("GScale y", &go->transform.scale.y, 0.01f);
	ImGui::DragFloat("GScale z", &go->transform.scale.z, 0.01f);


	if (ImGui::Button("Print GameObject matrix")) {
		Hazard::Matrix4 matrix;
		matrix.InitTranslation(go->transform.position);
		std::cout << matrix.ToString() << std::endl;
	}

	ImGui::End();
}

