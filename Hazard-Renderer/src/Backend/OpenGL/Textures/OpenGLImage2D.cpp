
#include "OpenGLImage2D.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/OpenGL/OpenGLUtils.h"
#include "Backend/Core/Renderer.h"

#include <glad/glad.h>

namespace HazardRenderer::OpenGL
{
	OpenGLImage2D::OpenGLImage2D(Image2DCreateInfo* info)
	{
		m_Width = info->Width;
		m_Height = info->Height;
		m_Format = info->Format;
		m_Samples = info->Mips;

		Invalidate();
		if (info->Data.Data != nullptr)
			SetImageData(info->Data);
	}
	OpenGLImage2D::~OpenGLImage2D()
	{
		Release();
	}
	void OpenGLImage2D::Invalidate()
	{
		if (m_ID)
		{
			Release();
		}

		bool multisampled = m_Samples > 1;
		OpenGLUtils::CreateTextures(multisampled, &m_ID, 1);
	}
	void OpenGLImage2D::Release()
	{
		glDeleteTextures(1, &m_ID);
		m_ID = 0;
	}
	void OpenGLImage2D::Bind(uint32_t slot)
	{
		Renderer::Submit([s = slot, id = m_ID]() mutable {
			glBindTextureUnit(s, id);
			});
	}
	void OpenGLImage2D::SetImageData(const Buffer& buffer)
	{
		glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height);
		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.Data);
		HZR_ASSERT(m_Width && m_Height, "This broke");
	}
}
#endif
