#pragma once

#include "Core/Image2D.h"

using namespace HazardUtility;

namespace HazardRenderer::OpenGL
{
	class OpenGLImage2D : public Image2D {
	public:
		OpenGLImage2D(Image2DCreateInfo* info);
		~OpenGLImage2D();

		void Invalidate() override;
		void Release() override;

		uint32_t GetWidth() { return m_Width; };
		uint32_t GetHeight() { return m_Height; };
		ImageFormat GetFormat() const override { return m_Format; }

		virtual float GetAspectRatio() { return (float)m_Width / (float)m_Height; };
		virtual Buffer GetBuffer() const { return Buffer(); };
		virtual const Buffer& GetBuffer() { return Buffer(); };
		
		uint32_t GetID() const { return m_ID; }

	private:
		uint32_t m_ID;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_Samples;
		ImageFormat m_Format;
	};
}