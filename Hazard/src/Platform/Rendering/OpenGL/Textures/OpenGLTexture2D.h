#pragma once

#include "Hazard/Rendering/Texture.h"


namespace Hazard::Rendering::OpenGL
{
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(Texture2DCreateInfo* info);
		~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width; };
		uint32_t GetHeight() const override { return m_Height; };

		uint32_t GetID() const override { return m_ID; }

		virtual float GetAspectRatio() { return (float)m_Width / (float)m_Height; }
		virtual Buffer GetBuffer() const { return Buffer(); };
		virtual Buffer& GetBuffer() { return Buffer(); }

	private:

		void LoadFromFile(const std::string& path);
		void LoadFromData(const void* data, uint32_t width, uint32_t height);
		void SetFilters(TextureFilter* filters, ImageWrap wrap);

		uint32_t m_Format;
		uint32_t m_InternalFormat;

		int m_Width;
		int m_Height;
		uint32_t m_ID = 0;
	};
}