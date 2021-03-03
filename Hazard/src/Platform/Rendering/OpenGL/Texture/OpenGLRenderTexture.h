#pragma once

#include "Hazard/Rendering/Textures/RenderTexture.h"
#include "glad/glad.h"

namespace Hazard::Rendering {

	class HAZARD_API OpenGLRenderTexture : public RenderTexture {
	public:

		OpenGLRenderTexture();
		virtual ~OpenGLRenderTexture();

		void Invalidate();

		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot = 0) const override;

		void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetWidth() const override { return spec.width; }
		virtual uint32_t GetHeight() const override { return spec.height; }

		uint32_t GetColorID() const { return colorAttachment; }
		uint32_t GetID() const override { return RendererID; }
		RenderTextureSpec& GetSpec() { return spec; }
		virtual void SetData(void* data, uint32_t size) {}

	private:
		RenderTextureSpec spec;

		GLuint RendererID = 0;
		GLuint colorAttachment = 0;
		GLuint depthAttachment = 0;

	};
}