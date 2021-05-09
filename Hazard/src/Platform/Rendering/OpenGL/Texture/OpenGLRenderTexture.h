#pragma once

#include "Hazard/Rendering/Textures/RenderTexture.h"
#include "glad/glad.h"

namespace Hazard::Rendering::OpenGL {

	class OpenGLRenderTexture : public RenderTexture {
	public:

		OpenGLRenderTexture(TextureSpecs params = TextureSpecs());
		virtual ~OpenGLRenderTexture();

		void Invalidate();

		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot = 0) const override;

		void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetWidth() const override { return m_Spec.width; }
		virtual uint32_t GetHeight() const override { return m_Spec.height; }

		uint32_t GetColorID() const { return m_ColorAttachment; }
		uint32_t GetID() const override { return m_RendererID; }

		TextureSpecs& GetSpec() override { return m_Spec; }
		virtual void SetData(void* data, uint32_t size) {}

	private:
		TextureSpecs m_Spec;

		GLuint m_RendererID = 0;
		GLuint m_ColorAttachment = 0;
		GLuint m_DepthAttachment = 0;

	};
}