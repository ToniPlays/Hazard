#pragma once

#include <hzrpch.h>
#include "OpenGLFrameBuffer.h"
#include "../OpenGLUtils.h"

namespace Hazard::Rendering::OpenGL {

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferCreateInfo& info)
	{
		m_Info.width  = info.width;
		m_Info.height = info.height;
		m_Info.samples = info.samples;

		for (auto format : info.attachments) {
			if (!OpenGLUtils::IsDepthFormat(format.format)) {
				m_Info.colorAttachments.emplace_back(format);
			}
			else m_Info.depthAttachment = format;
		}

		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);

		if (m_ColorIDs.size() > 0) {
			glDeleteTextures(m_ColorIDs.size(), m_ColorIDs.data());
			m_ColorIDs.clear();
		}


		if(m_DepthID != 0)
			glDeleteTextures(1, &m_DepthID);
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_Info.height <= 0 || m_Info.height >= 8192 ||
			m_Info.width <= 0 || m_Info.width >= 8192) return;

		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			if (m_ColorIDs.size() > 0) {
				glDeleteTextures(m_ColorIDs.size(), m_ColorIDs.data());
			}
			m_ColorIDs.clear();
			glDeleteTextures(1, &m_DepthID);
			m_DepthID = 0;
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisampled = m_Info.samples > 1;
		if (m_Info.colorAttachments.size() > 0) {

			m_ColorIDs.resize(m_Info.colorAttachments.size());

			OpenGLUtils::CreateTextures(multisampled, m_ColorIDs.data(), m_Info.colorAttachments.size());
			for (size_t i = 0; i < m_ColorIDs.size(); i++)
			{
				OpenGLUtils::BindTexture(multisampled, m_ColorIDs[i]);
				switch (m_Info.colorAttachments[i].format)
				{
				case FrameBufferTextureFormat::RGBA8:
					OpenGLUtils::AttachColorTexture(m_ColorIDs[i], m_Info.samples, GL_RGBA8, m_Info.width, m_Info.height, i);
					break;
				}
			}
		}

		if (m_Info.depthAttachment.format != FrameBufferTextureFormat::None) {
			OpenGLUtils::CreateTextures(multisampled, &m_DepthID, 1);
			OpenGLUtils::BindTexture(multisampled, m_DepthID);

			switch (m_Info.depthAttachment.format)
			{
			case FrameBufferTextureFormat::DEPTH32_STENCIL8:
				OpenGLUtils::AttachDepthTexture(m_DepthID, m_Info.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Info.width, m_Info.height);
				break;
			}
		}

		if (m_ColorIDs.size() > 1) {
			HZR_CORE_ASSERT(m_ColorIDs.size() <= 4, "Too many color attachments");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorIDs.size(), buffers);
		}
		else if(m_ColorIDs.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		HZR_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Bind(uint32_t slot) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Info.width, m_Info.height);
	}

	void OpenGLFrameBuffer::Unbind(uint32_t slot) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
			return;

		m_Info.width = width;
		m_Info.height = height;

		Invalidate();
	}
}
