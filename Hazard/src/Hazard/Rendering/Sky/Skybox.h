#pragma once

#include "../RenderUtils.h"
#include <glm/glm.hpp>

namespace Hazard::Rendering {

	class Skybox {
	public:

		Skybox();
		~Skybox();

		void SetCubemapTexture(CubemapTexture* texture) { this->m_Texture = texture; }
		void Render(glm::mat4 transform);
		
	private:
		VertexArray* m_VAO = nullptr;
		CubemapTexture* m_Texture = nullptr;
		Shader* m_SkyboxShader = nullptr;
	};
}