#pragma once

#include "../RenderUtils.h"

namespace Hazard::Rendering {
	class Material {
	public:
		Material(Shader* shader);
		~Material();

		Shader& GetShader() { return *shader; }
		std::vector<Texture2D*> GetTextures() { return textures; }
		void SetTextures(std::vector<Texture2D*> textures) { this->textures = textures; };

	public:
		static Material* Create(const char* shader);

	private:
		Shader* shader;
		std::vector<Texture2D*> textures;
	};
}