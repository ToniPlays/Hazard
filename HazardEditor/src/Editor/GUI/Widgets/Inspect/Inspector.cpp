#pragma once

#pragma once
#include <hzreditor.h>
#include "Inspector.h"
#include "Editor/Core/Analytics/Debug.h"

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

	ImGui::End();
}

