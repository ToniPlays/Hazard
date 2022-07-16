#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/Texture/TextureFactory.h"

namespace HazardRenderer::OpenGL
{
/*
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(Texture2DCreateInfo* info);
		~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width; };
		uint32_t GetHeight() const override { return m_Height; };
		virtual void Bind(uint32_t slot = 0) const override;

		virtual void Resize(uint32_t width, uint32_t height) {};
		virtual Buffer GetWriteBuffer() { return Buffer(); };
		virtual bool Loaded() const { return m_Loaded; };
		virtual ImageFormat GetFormat() const override { return m_Format; }

		uint32_t GetID() const { return m_ID; }

		virtual float GetAspectRatio() { return (float)m_Width / (float)m_Height; }
		virtual Buffer GetBuffer() const { return Buffer(); };
		virtual const Buffer& GetBuffer() { return Buffer(); }

	private:

		void LoadFromFile(const std::string& path);
		void LoadFromData(const void* data, uint32_t width, uint32_t height);
		void SetFilters(TextureFilter* filters, ImageWrap wrap);

		uint32_t m_InternalFormat;
		uint32_t m_DataFormat;

		Buffer m_LocalData;

		ImageFormat m_Format;
		bool m_Loaded = false;

		int m_Width;
		int m_Height;
		uint32_t m_ID = 0;
	};*/
}
#endif
