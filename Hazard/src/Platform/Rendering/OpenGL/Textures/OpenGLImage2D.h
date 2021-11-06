#pragma once

#include "Hazard/Rendering/Image/Image2D.h"


namespace Hazard::Rendering::OpenGL
{
	class OpenGLImage2D : public Image2D {
	public:
		OpenGLImage2D(Image2DCreateInfo* info);
		~OpenGLImage2D();

		virtual void Invalidate() {};
		virtual void Release() {};

		uint32_t GetWidth() override { return m_Width; };
		uint32_t GetHeight() override { return m_Height; };

		void* GetID() const override { return (void*)m_ID; }

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