#pragma once

#include <hzrpch.h>
#include "OpenGLRenderTexture.h"
#include "../OpenGLUtils.h"

namespace Hazard::Rendering::OpenGL {

	OpenGLRenderTexture::OpenGLRenderTexture(TextureSpecs params) : RenderTexture("RenderTexture")
	{
		m_Spec.width = params.width != -1 ? params.width : 1920;
		m_Spec.height = params.height != -1 ? params.width : 1080;
		m_Spec.samples = params.samples;

		Invalidate();
	}

	OpenGLRenderTexture::~OpenGLRenderTexture()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLRenderTexture::Invalidate()
	{
		if (m_Spec.height <= 0 || m_Spec.height >= 8192 || m_Spec.width <= 0 || m_Spec.width >= 8192) return;

		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_ColorAttachment);
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

		GLuint dataType = m_Spec.dataType == TextureDataType::Auto ? GL_RGBA8 : OpenGLUtils::DataTypeToOpenGLType(m_Spec.dataType);

		glTexImage2D(GL_TEXTURE_2D, 0, dataType, m_Spec.width, m_Spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Spec.width, m_Spec.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		HZR_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLRenderTexture::Bind(uint32_t slot) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Spec.width, m_Spec.height);
	}

	void OpenGLRenderTexture::Unbind(uint32_t slot) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLRenderTexture::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
			return;

		m_Spec.width = width;
		m_Spec.height = height;

		Invalidate();
	}
}
