#pragma once

#include "RenderUtils.h"
#include <glm/glm.hpp>

namespace Hazard::Rendering {

	class Skybox {
	public:

		Skybox();
		~Skybox();

		void SetCubemapTexture(CubemapTexture* texture) { this->texture = texture; }
		void Render(glm::mat4 transform);
		
	private:
		VertexArray* vao = nullptr;
		CubemapTexture* texture = nullptr;
		Shader* skyboxShader = nullptr;
	};
}