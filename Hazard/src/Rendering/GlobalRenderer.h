#pragma once
#include "Core/Core.h"
#include "Modules/Module/Module.h"
#include "glad/glad.h"
#include "Rendering/Textures/RenderTexture.h"
#include "Rendering/Textures/DefaultRenderTexture.h"

namespace Hazard {
	class HAZARD_API GlobalRenderer : public Module {
	public:
		GlobalRenderer();
		~GlobalRenderer() {};
		void SceneRender();
		void DefaultTarget() { renderTarget = defaultTarget; }

		RenderTexture* GetRenderTexture() { return renderTarget != nullptr ? renderTarget : (RenderTexture*)defaultTarget; }
		RenderTexture* renderTarget = nullptr;
		DefaultRenderTexture* defaultTarget = new DefaultRenderTexture();
	private:

		GLuint vertexArray;
		GLuint vertexBuffer;
		GLuint indiceBuffer;
		GLuint texture;

	};
}