#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/Texture/Image2D.h"

namespace HazardRenderer::OpenGL
{
	class OpenGLImage2D : public Image2D {
	public:
		OpenGLImage2D(Image2DCreateInfo* info);
		~OpenGLImage2D();

		void Invalidate() override;
		void Release() override;

		virtual void Bind(uint32_t slot = 0) override;

		uint32_t GetWidth() override { return m_Width; };
		uint32_t GetHeight() override { return m_Height; };
		ImageFormat GetFormat() const override { return m_Format; }

		virtual float GetAspectRatio() override { return (float)m_Width / (float)m_Height; };
		virtual Buffer GetBuffer() const override { return m_LocalBuffer; };
		virtual const Buffer& GetBuffer() override { return m_LocalBuffer; };
		
		size_t GetID() const { return m_ID; }
		const std::string& GetDebugName() const { return m_DebugName; }
		
	private:
		void SetImageData(const Buffer& buffer);

	private:
		std::string m_DebugName;
		uint32_t m_ID = 0;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_Samples = 0;
		ImageFormat m_Format = ImageFormat::None;
        Buffer m_LocalBuffer;
	};
}
#endif
