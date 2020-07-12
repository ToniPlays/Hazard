#pragma once

#pragma once
#include <hzreditor.h>
#include "Inspector.h"
#include "Editor/Core/Analytics/Debug.h"


Inspector::Inspector()
{
	SetLayerOpen(true);
}


void Inspector::Render()
{
	if (!isLayerOpen) return;

	ImGui::Begin("Inspector", &isLayerOpen);
	Hazard::GlobalRenderer* renderer = Hazard::ModuleHandler::GetModule<Hazard::GlobalRenderer>();
	std::stringstream ss;

	ss << "Rendering " << Hazard::Math::Pow(renderer->Get2D()->size, 2) << " quads";

	ImGui::Text(ss.str().c_str());

	ImGui::Text("Camera");

	Hazard::Camera* c = Hazard::Camera::instance;
	ImGui::SliderFloat("Zoom level", &c->size, 0, 100);
	ImGui::SetNextItemWidth(100);
	ImGui::DragFloat("X", &c->transform.position.x, 0.1f);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100);
	ImGui::DragFloat("Y", &c->transform.position.y, 0.1f);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100);
	ImGui::DragFloat("Z", &c->transform.position.z, 0.1f);

	ImGui::End();
}

