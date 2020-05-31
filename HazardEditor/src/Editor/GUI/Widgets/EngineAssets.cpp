#pragma once
#include "EngineAssets.h"
#include "imgui.h"

bool EngineAssets::layerOpen = true;

std::string ToString(Hazard::TextureType type) {

	switch (type)
	{
	case Hazard::TextureType::T2D:
		return "Texture 2D";
	case Hazard::TextureType::Render:
		return "RenderTexture";
	default:
		return "Unknown texture type";
	}
}

EngineAssets::EngineAssets()
{
}


void EngineAssets::Render()
{
	if (!layerOpen) return;

	ImGui::Begin("Engine assets", &layerOpen);

	if(ImGui::CollapsingHeader("Loaded textures")) {
		std::unordered_map<Hazard::TextureType, std::vector<Hazard::Texture*>> textures = Hazard::TextureManager::GetTextures();

		for (std::pair<Hazard::TextureType, std::vector<Hazard::Texture*>> type : textures) {
			if(ImGui::TreeNode(ToString(type.first).c_str())) {

				for (Hazard::Texture* texture : type.second) {
					std::stringstream ss;
					ss << texture->GetPath();
					ss << "\n" << texture->GetWidth() << "x" << texture->GetHeight();

					ImGui::Image((ImTextureID)texture->GetID(), ImVec2(50, 50));
					ImGui::SameLine();
					ImGui::Text(ss.str().c_str());
				}
				ImGui::TreePop();
			}
		}
	}

	ImGui::End();
}
