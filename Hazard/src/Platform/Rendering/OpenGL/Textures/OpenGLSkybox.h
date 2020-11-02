#pragma once
#include "Hazard/Modules/Renderer/Textures/Skybox.h"
#include "OpenGLCubemapTexture.h"
#include "Hazard/Modules/Renderer/RendererAPI.h"

#include <glad/glad.h>

namespace Hazard {
	class OpenGLSkybox : public Skybox {
	public:
		OpenGLSkybox();
		~OpenGLSkybox();

		void Render();
		void SetCubemap(CubemapTexture* texture);
	private:
		Shader* skyboxShader = nullptr;
		CubemapTexture* texture = nullptr;
		VertexArray* vertexArray = nullptr;
	};
}