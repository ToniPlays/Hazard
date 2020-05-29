#pragma once

#include <hzrpch.h>
#include "TextureManager.h"

namespace Hazard {
	
	std::unordered_map<TextureType, std::vector<Texture*>> TextureManager::textures;

	TextureManager::TextureManager()
	{

	}

	TextureManager::~TextureManager()
	{
		for (std::pair<TextureType, std::vector<Texture*>> type : textures) {
			for (Texture* texture : type.second) {
				delete texture;
			}
		}
	}

	void TextureManager::AddTexture(TextureType type, Texture* texture)
	{
		if (textures.count(type) == 0) {
			textures[type] = std::vector<Texture*>();
		}
		textures[type].push_back(texture);
	}

	Texture* TextureManager::GetTexture(TextureType type, std::string file)
	{
		if (textures.count(type) == 0) return nullptr;
		
		for (Texture* texture : textures[type]) {
			if (texture->GetPath() == file) return texture;
		}
		return nullptr;
	}

}
