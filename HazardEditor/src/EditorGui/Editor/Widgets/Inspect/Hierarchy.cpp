#pragma once
#include "HzrEditor.h"
#include "Hierarchy.h"
#include "imgui_internal.h"
#include "EditorGui/Editor/EditorGUI.h"

bool Hierarchy::layerOpen = true;

void Hierarchy::OnRender() const
{
	if (!layerOpen) return;
	ImGui::Begin("Hierarchy", &layerOpen);

	ImGui::Text("Scene");

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
