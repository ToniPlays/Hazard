#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/Image2D.h"

namespace HazardRenderer::OpenGL
{
	class OpenGLImage2D : public Image2D 
	{
	public:
		OpenGLImage2D(Image2DCreateInfo* info);
		~OpenGLImage2D();

		void Invalidate() override;
		void Release() override;

		Buffer ReadPixels(const ImageCopyRegion& region) { return Buffer(); };

		const std::string& GetDebugName() const override { return m_DebugName; };

		uint32_t GetWidth() override { return m_Extent.Width; };
		uint32_t GetHeight() override { return m_Extent.Height; };
		ImageFormat GetFormat() const override { return m_Format; }
		virtual uint32_t GetMipLevels() const override { return 0; };

		virtual float GetAspectRatio() override { return (float)m_Extent.Width / (float)m_Extent.Height; };
		virtual Buffer GetBuffer() const override { return m_LocalBuffer; };
		virtual const Buffer& GetBuffer() override { return m_LocalBuffer; };
		
		void Invalidate_RT();
		void Release_RT();
		void SetData_RT();

		//OpenGL specific
		uint64_t GetID() const { return m_ID; }
		
	private:
		void SetImageData(const Buffer& buffer);

	private:
		std::string m_DebugName;
		uint32_t m_ID = 0;
		Extent m_Extent;
		uint32_t m_Samples = 0;
		uint32_t m_MipLevels = 0;
		ImageUsage m_ImageUsage;
		ImageFormat m_Format = ImageFormat::None;
        Buffer m_LocalBuffer;
	};
}
#endif
