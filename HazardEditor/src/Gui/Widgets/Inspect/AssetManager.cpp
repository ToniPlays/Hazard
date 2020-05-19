#pragma once
#include "Hazard.h"
#include "AssetManager.h"
#include "imgui_internal.h"
#include "imgui.h"

bool AssetManager::layerOpen = true;

void AssetManager::OnRender() const
{
	if (!layerOpen) return;
	ImGui::Begin("Assets", &layerOpen);

	ImGui::Text("Assets");

	ImGui::End();
}

void AssetManager::OpenLayer() const
{
	//Debug::Log("Opened AssetManager");
	layerOpen = true;
}

void AssetManager::CloseLayer() const
{
	//Debug::Log("Closed AssetManager");
	layerOpen = true;
}
