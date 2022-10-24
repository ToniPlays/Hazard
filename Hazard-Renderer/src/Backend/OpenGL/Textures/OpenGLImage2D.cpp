
#include "OpenGLImage2D.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/OpenGL/OpenGLUtils.h"
#include "Backend/Core/Renderer.h"

#include <glad/glad.h>

namespace HazardRenderer::OpenGL
{
	OpenGLImage2D::OpenGLImage2D(Image2DCreateInfo* info)
	{
		HZR_ASSERT(!info->DebugName.empty(), "Debug name required");
		m_DebugName = info->DebugName;
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
		HZR_PROFILE_FUNCTION();
		Release_RT();
	}
	void OpenGLImage2D::Invalidate()
	{
		HZR_PROFILE_FUNCTION();
		if (m_ID)
			Release();

		m_ID = OpenGLUtils::CreateTextures(m_Samples > 1, 1);
		/*
		Ref<OpenGLImage2D> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			
			});
			*/
	}
	void OpenGLImage2D::Release()
	{
		Ref<OpenGLImage2D> instance = this;
		Renderer::Submit([instance]() mutable {
			instance->Release_RT();
			});
	}
	void OpenGLImage2D::Release_RT()
	{
		HZR_PROFILE_FUNCTION();
		if (m_ID == 0) return;

		glDeleteTextures(1, &m_ID);
		m_LocalBuffer.Release();
		m_ID = 0;
	}
	void OpenGLImage2D::SetImageData(const Buffer& buffer)
	{
		HZR_PROFILE_FUNCTION();
		uint32_t format = OpenGLUtils::GetGLFormat(m_Format);
		glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height);
		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.Data);
		HZR_ASSERT(m_Width && m_Height, "This broke");
	}
}
#endif
