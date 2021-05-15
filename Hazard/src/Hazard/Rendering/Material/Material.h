#pragma once

#include "../RenderUtils.h"

namespace Hazard::Rendering {

	struct MaterialCreateInfo {
		ShaderCreateInfo* shader;
	};


	class Material : public RefCount {
	public:
		Material(const Ref<Shader>& shader);
		~Material();

		Ref<Shader> GetShader() { return m_Shader; }
		std::vector<Ref<Texture2D>> GetTextures() { return m_Textures; }
		void SetTextures(const std::vector<Ref<Texture2D>>& textures) { this->m_Textures = textures; };

	public:
		static Ref<Material> Create(const MaterialCreateInfo& info);

	private:
		Ref<Shader> m_Shader;
		std::vector<Ref<Texture2D>> m_Textures;
	};
}