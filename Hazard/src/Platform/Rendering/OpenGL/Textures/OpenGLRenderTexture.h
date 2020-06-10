#pragma once

#include "Hazard/Modules/Renderer/Textures/RenderTexture.h"
#include "glad/glad.h"

namespace Hazard {

	class HAZARD_API OpenGLRenderTexture : public RenderTexture {
	public:

		OpenGLRenderTexture();
		~OpenGLRenderTexture();

		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot = 0) const override;

		uint32_t GetID() const override { return RendererID; }
		uint32_t GetColorID() const override{ return colorAttachment; }


	private:
		void Create();

		GLuint RendererID = 0;
		GLuint colorAttachment = 0;
		GLuint depthAttachment = 0;
	};
}