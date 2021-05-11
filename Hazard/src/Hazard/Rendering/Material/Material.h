#pragma once

#include "../RenderUtils.h"

namespace Hazard::Rendering {
	class Material {
	public:
		Material(Ref<Shader> shader);
		~Material();

		Ref<Shader>& GetShader() { return m_Shader; }
		std::vector<Ref<Texture2D>> GetTextures() { return m_Textures; }
		void SetTextures(std::vector<Ref<Texture2D>> textures) { this->m_Textures = textures; };

	public:
		static Material* Create(const char* shader);

	private:
		Ref<Shader> m_Shader;
		std::vector<Ref<Texture2D>> m_Textures;
	};
}