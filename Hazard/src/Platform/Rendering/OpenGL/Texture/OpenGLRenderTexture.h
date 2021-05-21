#pragma once

#include "Hazard/Rendering/Textures/RenderTexture.h"
#include "glad/glad.h"

namespace Hazard::Rendering::OpenGL {

	class OpenGLRenderTexture : public RenderTexture {
	public:

		OpenGLRenderTexture(const RenderTextureCreateInfo& info);
		virtual ~OpenGLRenderTexture();


		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot = 0) const override;
		void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetWidth() const override { return 0; }
		virtual uint32_t GetHeight() const override { return 0; }

		RenderTextureInfo GetInfo() const override { return m_Info; }
		uint32_t GetID() const { return m_RendererID; };

		TextureInfo GetData() const override { return m_TextureInfo; };
	private:

		void Invalidate();
		RenderTextureInfo m_Info;
		TextureInfo m_TextureInfo;

		GLuint m_RendererID = 0;
		GLuint m_DepthAttachment = 0;
	};
}