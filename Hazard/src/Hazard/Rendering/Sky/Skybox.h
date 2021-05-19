#pragma once

#include "../RenderUtils.h"
#include <glm/glm.hpp>

namespace Hazard::Rendering {

	class Skybox : public RefCount {
	public:

		Skybox();
		~Skybox();

		Ref<CubemapTexture> GetCubemapTexture() { return m_Texture; }
		void SetCubemapTexture(Ref<CubemapTexture>& texture) { this->m_Texture = texture; }
		void Render();

		float GetGamma() { return m_Gamma; }
		void SetGamma(float gamma) { m_Gamma = gamma; }
		
	private:
		float m_Gamma = 1.0f;
		VertexArray* m_VAO;
		Ref<CubemapTexture> m_Texture;
		Ref<Shader> m_SkyboxShader;
	};
}