#pragma once
#include <hzreditor.h>
#include "EngineAssets.h"

using namespace Hazard;

EngineAssets::EngineAssets() : Layer("Engine assets")
{

}

void EngineAssets::Render()
{
	if (!Panel::Begin(name, isLayerOpen)) return;
	std::vector<TextureData*> textures = AssetManager::GetAssets<Hazard::TextureData>();
	
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | 
		ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	
	if (textures.size() > 0) {
		if (ImGui::TreeNodeEx("Textures", flags, "Textures")) {

			for (TextureData* texture : textures) {

			}

			ImGui::TreePop();
		}
	}
	std::vector<ShaderData*> shaderData = AssetManager::GetAssets<Hazard::ShaderData>();
	if (shaderData.size() > 0) {
		if (ImGui::TreeNodeEx("##Shader", flags, "Shaders")) {

			for (ShaderData* shaders : shaderData) {
				for (Shader* shader : shaders->shaders) {
					if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)shader->file.c_str(), flags, shader->file.c_str())) {
						ImGui::Text(shader->file.c_str());
						ImGui::TreePop();
					}
				}
			}

			ImGui::TreePop();
		}
	}
	Panel::End();
}
