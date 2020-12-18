#pragma once
#include <hzreditor.h>
#include "Performance.h"
#include "Editor/GUI/GUILibrary/Inputs.h"

static bool vsync;
Performance::Performance() : Layer("Performance") {}

Performance::~Performance()
{
}

bool Performance::OnEnabled()
{
	return true;
}

void Performance::Render()
{
	if (!Panel::Begin(name, isLayerOpen)) return;

	ImGui::Columns(2);

	ImGui::Text("Frametime");
	ImGui::NextColumn();
	ImGui::Text((std::to_string(Hazard::Time::deltaTime) + "ms").c_str());
	ImGui::NextColumn();
	ImGui::Text("FPS");
	ImGui::NextColumn();
	ImGui::Text(std::to_string((uint32_t(1 / Hazard::Time::deltaTime))).c_str());

	ImGui::Columns(1);

	Panel::End();
}
