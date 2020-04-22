#pragma once
#include "HzrEditor.h"
#include "AssetManager.h"
#include "imgui_internal.h"
#include "EditorGui/Editor/EditorGUI.h"

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
	Debug::Log("Opened AssetManager");
	layerOpen = true;
}

void AssetManager::CloseLayer() const
{
	Debug::Log("Closed AssetManager");
	layerOpen = true;
}
