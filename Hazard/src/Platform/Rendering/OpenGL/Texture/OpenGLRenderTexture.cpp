#pragma once

#include <hzrpch.h>
#include "OpenGLRenderTexture.h"
#include "../OpenGLUtils.h"

namespace Hazard::Rendering::OpenGL {

	OpenGLRenderTexture::OpenGLRenderTexture(const RenderTextureCreateInfo& info)
	{
		m_TextureInfo.width  = info.width;
		m_TextureInfo.height = info.height;
		m_TextureInfo.datatype = info.datatype;
		m_TextureInfo.file = info.name;

		Invalidate();
	}

	OpenGLRenderTexture::~OpenGLRenderTexture()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_Info.colorID);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLRenderTexture::Invalidate()
	{
		if (m_TextureInfo.height <= 0 || m_TextureInfo.height >= 8192 ||
			m_TextureInfo.width <= 0 || m_TextureInfo.width >= 8192) return;

		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_Info.colorID);
			glDeleteTextures(1, &m_Info.colorID);
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Info.colorID);
		glBindTexture(GL_TEXTURE_2D, m_Info.colorID);

		GLuint dataType = m_TextureInfo.datatype == TextureDataType::Auto ? GL_RGBA8 : OpenGLUtils::DataTypeToOpenGLType(m_TextureInfo.datatype);

		glTexImage2D(GL_TEXTURE_2D, 0, dataType, m_TextureInfo.width, m_TextureInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Info.colorID, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_TextureInfo.width, m_TextureInfo.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		HZR_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	}

	void OpenGLRenderTexture::Bind(uint32_t slot) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_TextureInfo.width, m_TextureInfo.height);
	}

	void OpenGLRenderTexture::Unbind(uint32_t slot) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLRenderTexture::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
			return;

		m_TextureInfo.width = width;
		m_TextureInfo.height = height;

		Invalidate();
	}
}
