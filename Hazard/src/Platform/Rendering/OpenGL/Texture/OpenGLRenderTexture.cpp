#pragma once

#include <hzrpch.h>
#include "OpenGLRenderTexture.h"

namespace Hazard::Rendering {

	OpenGLRenderTexture::OpenGLRenderTexture() : RenderTexture()
	{
		spec.width = 1920;
		spec.height = 1080;
		Invalidate();
	}

	OpenGLRenderTexture::~OpenGLRenderTexture()
	{
		glDeleteFramebuffers(1, &RendererID);
		glDeleteTextures(1, &colorAttachment);
		glDeleteTextures(1, &depthAttachment);
	}

	void OpenGLRenderTexture::Invalidate()
	{

		if (spec.height <= 0 || spec.height >= 8192 || spec.width <= 0 || spec.width >= 8192) return;
		if (RendererID)
		{
			glDeleteFramebuffers(1, &RendererID);
			glDeleteTextures(1, &colorAttachment);
			glDeleteTextures(1, &colorAttachment);
		}

		glGenFramebuffers(1, &RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
		glBindTexture(GL_TEXTURE_2D, colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec.width, spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
		glBindTexture(GL_TEXTURE_2D, depthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, spec.width, spec.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

		HZR_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLRenderTexture::Bind(uint32_t slot) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, RendererID);
		glViewport(0, 0, spec.width, spec.height);
	}

	void OpenGLRenderTexture::Unbind(uint32_t slot) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLRenderTexture::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
			return;

		spec.width = width;
		spec.height = height;

		Invalidate();
	}
}
