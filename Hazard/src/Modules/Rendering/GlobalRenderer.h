#pragma once
#include "Core/Core.h"
#include "Modules/Module/Module.h"
#include "glad/glad.h"
#include "Shaders/OpenGLShader.h"
#include "Textures/RenderTexture.h"
#include "Textures/DefaultRenderTexture.h"

namespace Hazard {
	class HAZARD_API GlobalRenderer : public Module {
	public:
		GlobalRenderer();
		~GlobalRenderer() {};
		void SceneRender();
		void DefaultTarget() { renderTarget = nullptr; }

		RenderTexture* GetRenderTexture() { return renderTarget != nullptr ? renderTarget : (RenderTexture*)defaultTarget; }
		RenderTexture* renderTarget = nullptr;
	private:
		DefaultRenderTexture* defaultTarget;
		Shader* shader;
		uint32_t vertexArray;
		uint32_t vertexBuffer;
		uint32_t colorBuffer;
		uint32_t indiceBuffer;
		uint32_t texture;

	};
}