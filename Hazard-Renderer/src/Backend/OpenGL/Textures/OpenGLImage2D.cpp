
#include "OpenGLImage2D.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/OpenGL/OpenGLUtils.h"
#include "Backend/Core/Renderer.h"

#include <glad/glad.h>

namespace HazardRenderer::OpenGL
{
	OpenGLImage2D::OpenGLImage2D(Image2DCreateInfo* info)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(!info->DebugName.empty(), "Debug name required");
		HZR_ASSERT(info->Format != ImageFormat::None, "Image format cannot be none");
		HZR_ASSERT(info->Usage != ImageUsage::None, "Image format cannot be none");

		m_DebugName = info->DebugName;
		m_Width = info->Width;
		m_Height = info->Height;
		m_Format = info->Format;
		m_Samples = 1;
		m_ImageUsage = info->Usage;
		m_MipLevels = info->GenerateMips ? OpenGLUtils::GetMipLevelCount(m_Width, m_Height) : 1;

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

		Ref<OpenGLImage2D> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
			});

		/*
		Ref<OpenGLImage2D> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {

			});
			*/
	}
	void OpenGLImage2D::Release()
	{
		HZR_PROFILE_FUNCTION();
		Ref<OpenGLImage2D> instance = this;
		Renderer::SubmitResourceFree([instance]() mutable {
			instance->Release_RT();
			});
	}
	void OpenGLImage2D::Invalidate_RT()
	{
		HZR_PROFILE_FUNCTION();
		HZR_RENDER_THREAD_ONLY();


		m_ID = OpenGLUtils::CreateTextures(m_Samples > 1);
		if (m_ImageUsage == ImageUsage::Attachment) return;

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, m_MipLevels > 1 ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (m_MipLevels > 1)
			glGenerateTextureMipmap(m_ID);
		/*
		/*

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/
	}
	void OpenGLImage2D::Release_RT()
	{
		HZR_PROFILE_FUNCTION();
		m_LocalBuffer.Release();

		if (m_ID == 0) return;

		glDeleteTextures(1, &m_ID);
		m_ID = 0;
	}
	void OpenGLImage2D::SetData_RT()
	{
		HZR_PROFILE_FUNCTION();
		HZR_RENDER_THREAD_ONLY();

		glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height);
		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer.Data);
		if (m_MipLevels > 1) {}
		//glGenerateTextureMipmap(m_ID);
	/*
	glBindTexture(GL_TEXTURE_2D, m_ID);

	for (uint32_t i = 1; i < m_MipLevels; i++)
	{
		glTextureStorage2D(m_ID, i, GL_RGBA8, (m_Width >> (i - 1)), (m_Height >> (i -1)));
	}
	*/

		m_LocalBuffer.Release();
	}
	void OpenGLImage2D::SetImageData(const Buffer& buffer)
	{
		HZR_PROFILE_FUNCTION();

		m_LocalBuffer.Release();
		m_LocalBuffer = Buffer::Copy(buffer);

		Ref<OpenGLImage2D> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->SetData_RT();
			});
	}
}
#endif
