#pragma once
#include <hzreditor.h>
#include "EngineAssets.h"
#include "Editor/GUI/GUILibrary/Inputs.h"

using namespace Hazard;

EngineAssets::EngineAssets() : Layer("Engine assets")
{

}

EngineAssets::~EngineAssets()
{
}

void EngineAssets::Render()
{
	if (!Panel::Begin(name, isLayerOpen)) return;
/*
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | 
		ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	
	if (textures.size() > 0) {
		if (ImGui::TreeNodeEx("Textures", flags, "Textures")) {

			for (TextureData* texture : textures) {
				if (ImGui::TreeNodeEx(TextureTypeToString(texture->type), flags)) {
					DrawTextures(texture);
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}
	std::vector<Shader*> shaders = AssetManager::GetAssets<Hazard::Shader>();
	if (shaders.size() > 0) {
		if (ImGui::TreeNodeEx("##Shader", flags, "Shaders")) {
			for (Shader* shader : shaders) {
				if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)shader->file.c_str(), flags, shader->file.c_str())) {
					ImGui::Text(shader->file.c_str());
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}
	*/
	Panel::End();
}
