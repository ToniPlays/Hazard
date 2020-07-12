#pragma once

#pragma once
#include <hzreditor.h>
#include "Inspector.h"
#include "Editor/Core/Analytics/Debug.h"


Inspector::Inspector()
{
}


void Inspector::Render()
{
	if (!isLayerOpen) return;

	ImGui::Begin("Inspector", &isLayerOpen);
	Hazard::GlobalRenderer* renderer = Hazard::ModuleHandler::GetModule<Hazard::GlobalRenderer>();
	ImGui::Text("GameObjects");

	ImGui::End();
}

