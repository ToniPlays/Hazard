

#include "OpenGLFramebuffer.h"
#ifdef HZR_INCLUDE_OPENGL
#include "OpenGLUtils.h"
#include "OpenGLContext.h"
#include "Backend/Core/Window.h"

#include <glad/glad.h>

namespace HazardRenderer::OpenGL
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(FrameBufferCreateInfo* info)
	{
		m_Specs.DebugName = info->DebugName;

		m_Specs.SwapChainTarget = info->SwapChainTarget;
		m_Specs.ClearOnLoad = info->ClearOnLoad;
		m_Specs.Samples = info->Samples;
		m_Specs.pFrameBuffer = info->pFrameBuffer;
		m_Specs.AttachmentCount = info->AttachmentCount;
		m_Specs.Attachments = info->Attachments;
		m_Specs.ClearColor = info->ClearColor;
		m_Specs.Width = (uint32_t)info->Width;
		m_Specs.Height = (uint32_t)info->Height;

		if (info->Width == 0 || info->Height == 0 || m_Specs.SwapChainTarget) 
		{

		}

		for (auto& attachment : info->Attachments) 
		{
			//if (!OpenGLUtils::IsDepthFormat(attachment.Format)) {
			//	m_ColorAttachments.emplace_back(attachment);
			//}
			//else 
			m_DepthAttachment = attachment;
		}

		Invalidate();
	}
	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
		if (m_ColorImages.size() > 0) {
			for (auto& image : m_ColorImages)
				image->Release();
		}

		if (m_DepthImage) {
			m_DepthImage->Release();
		}
	}
	void OpenGLFrameBuffer::Bind() const
	{
		
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		if(m_Specs.SwapChainTarget) 
		{
			glm::vec4 color = OpenGLContext::GetInstance().GetClearColor();
			Window* window = OpenGLContext::GetInstance().GetWindow();
			glClearColor(color.r, color.g, color.b, color.a);
			glViewport(0, 0, window->GetWidth(), window->GetHeight());
		}
		else 
		{
			glClearColor(m_Specs.ClearColor.r, m_Specs.ClearColor.g, m_Specs.ClearColor.b, m_Specs.ClearColor.a);
			glViewport(0, 0, m_Specs.Width, m_Specs.Height);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height, bool force)
	{
		if (m_Specs.Width == width && m_Specs.Height == height && !force) 
			return;

		m_Specs.Width = width;
		m_Specs.Height = height;

		Invalidate();
	}
	void OpenGLFrameBuffer::Invalidate()
	{
		RT_Invalidate();
	}
	void OpenGLFrameBuffer::RT_Invalidate()
	{
		if (m_Specs.SwapChainTarget) return;

		if (m_ID) {
			glDeleteFramebuffers(1, &m_ID);

			if (m_ColorImages.size() > 0) {
				if (m_ColorImages.size() > 0) {
					for (auto& image : m_ColorImages)
						image->Release();
				}
			}
			m_ColorImages.clear();

			if (m_DepthImage) {
				m_DepthImage->Release();
			}
		}

		glCreateFramebuffers(1, &m_ID);

		bool multisampled = m_Specs.Samples > 1;

		if (m_ColorAttachments.size() > 0) {
			m_ColorImages.resize(m_ColorAttachments.size());

			Image2DCreateInfo imageInfo = {};
			imageInfo.Usage = ImageUsage::Attachment;
			imageInfo.Width = m_Specs.Width;
			imageInfo.Height = m_Specs.Height;
			imageInfo.Mips = m_Specs.Samples;

			for (uint32_t i = 0; i < m_ColorImages.size(); i++) {
				imageInfo.Format = m_ColorAttachments[i].Format;
				m_ColorImages[i] = Image2D::Create(&imageInfo).As<OpenGLImage2D>();
			}
	
			for (uint32_t i = 0; i < (uint32_t)m_ColorImages.size(); i++)
			{
				OpenGLUtils::BindTexture(m_ColorImages[i]->GetID(), multisampled);

				switch (m_ColorAttachments[i].Format) {
				case ImageFormat::RGBA: break;
					//OpenGLUtils::AttachColorTexture(m_ColorImages[i]->GetID(), m_Specs.Samples, GL_RGBA8, m_Specs.Width, m_Specs.Height, i);
				}
			}
		}
		if (m_DepthAttachment.Format != ImageFormat::None) {

			Image2DCreateInfo imageInfo = {};
			imageInfo.Usage = ImageUsage::Attachment;
			imageInfo.Width = m_Specs.Width;
			imageInfo.Height = m_Specs.Height;
			imageInfo.Mips = m_Specs.Samples;
			imageInfo.Format = m_DepthAttachment.Format;

			m_DepthImage = Image2D::Create(&imageInfo).As<OpenGLImage2D>();
			//OpenGLUtils::BindTexture(m_DepthImage->GetID(), imageInfo.Mips > 1);

			switch (m_DepthAttachment.Format)
			{
			case ImageFormat::DEPTH24STENCIL8:
				//OpenGLUtils::AttachDepthTexture(m_DepthImage->GetID(), m_Specs.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specs.Width, m_Specs.Height);
				break;
			}
		}
		if (m_ColorImages.size() > 1) {
			HZR_ASSERT(m_ColorImages.size() <= 4, "Too many color attachments");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((uint32_t)m_ColorImages.size(), buffers);
		}
		else if (m_ColorImages.empty()) 
		{
			glDrawBuffer(GL_NONE);
		}

		HZR_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
#endif
