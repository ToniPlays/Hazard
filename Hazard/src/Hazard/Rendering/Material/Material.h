#pragma once

#include "../RenderUtils.h"

namespace Hazard::Rendering {
	class Material {
	public:
		Material(Shader* shader);
		~Material();

		Shader& GetShader() { return *m_Shader; }
		std::vector<Texture2D*> GetTextures() { return m_Textures; }
		void SetTextures(std::vector<Texture2D*> textures) { this->m_Textures = textures; };

	public:
		static Material* Create(const char* shader);

	private:
		Shader* m_Shader;
		std::vector<Texture2D*> m_Textures;
	};
}