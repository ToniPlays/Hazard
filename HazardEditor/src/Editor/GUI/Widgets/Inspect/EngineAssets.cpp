#pragma once
#include <hzreditor.h>
#include "EngineAssets.h"
#include "Editor/GUI/GUILibrary/Inputs.h"

using namespace Hazard;

const char* TextureTypeToString(TextureType type) {
	switch (type) {
		case TextureType::Cubemap: return "Cubemap";
		case TextureType::RenderTexture: return "RenderTexture";
		case TextureType::Texture: return "Texture";
		case TextureType::Texture2D: return "Texture2D";
	}
}

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
				if (ImGui::TreeNodeEx(TextureTypeToString(texture->type), flags)) {
					DrawTextures(texture);
					ImGui::TreePop();
				}
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
void EngineAssets::DrawTextures(TextureData* data) {

	for(Hazard::Texture* texture : data->textures)
		Inputs::Texture(texture, Vector2<int>(75, 75));
}
