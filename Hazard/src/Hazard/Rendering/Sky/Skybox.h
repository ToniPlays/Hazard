#pragma once

#include "../RenderUtils.h"
#include <glm/glm.hpp>

namespace Hazard::Rendering {

	class Skybox {
	public:

		Skybox();
		~Skybox();

		CubemapTexture* GetCubemapTexture() { return m_Texture; }
		void SetCubemapTexture(CubemapTexture* texture) { this->m_Texture = texture; }
		void Render(glm::mat4 transform);

		float GetGamma() { return m_Gamma; }
		void SetGamma(float gamma) { m_Gamma = gamma; }
		
	private:

		float m_Gamma = 1.0f;
		VertexArray* m_VAO = nullptr;
		CubemapTexture* m_Texture = nullptr;
		Shader* m_SkyboxShader = nullptr;
	};
}