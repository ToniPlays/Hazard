#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Modules/Rendering/Textures/Texture.h"
#include <unordered_map>
#include <vector>

namespace Hazard {

	enum TextureType {
		T2D = 0, Render
	};

	class HAZARD_API TextureManager {
	public:
		TextureManager();
		~TextureManager();

		static void AddTexture(TextureType type, Texture* texture);
		static Texture* GetTexture(TextureType type, std::string file);
		static std::unordered_map<TextureType, std::vector<Texture*>> GetTextures() { return textures; }

	private:
		static std::unordered_map<TextureType, std::vector<Texture*>> textures;
	};
}