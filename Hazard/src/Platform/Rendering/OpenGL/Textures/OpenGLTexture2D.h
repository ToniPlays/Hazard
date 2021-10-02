#pragma once

#include "Hazard/Rendering/Texture/Texture2D.h"

#include "glad/glad.h"

namespace Hazard::Rendering::OpenGL
{
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(Texture2DCreateInfo* info);
		~OpenGLTexture2D();

		void Bind(uint32_t slot = 0) const override;

		uint32_t GetWidth() override { return m_Width; };
		uint32_t GetHeight() override { return m_Height; };

		void* GetID() const override { return (void*)m_ID; }

	private:

		void LoadFromFile(const std::string& path);
		void LoadFromData(const void* data, uint32_t width, uint32_t height);
		void SetFilters(TextureFilter* filters, TextureWrap wrap);

		GLuint m_Format;
		GLuint m_InternalFormat;

		int m_Width;
		int m_Height;
		uint32_t m_ID = 0;
	};
}