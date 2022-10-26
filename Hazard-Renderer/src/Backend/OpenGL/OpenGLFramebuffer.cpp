

#include "OpenGLFramebuffer.h"
#ifdef HZR_INCLUDE_OPENGL

#include "Backend/Core/Renderer.h"
#include "OpenGLUtils.h"
#include "OpenGLContext.h"
#include "Backend/Core/Window.h"

#include "spdlog/fmt/fmt.h"

#include <glad/glad.h>

namespace HazardRenderer::OpenGL
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(FrameBufferCreateInfo* info)
	{
		HZR_PROFILE_FUNCTION();
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

		if ((info->Width == 0 || info->Height == 0) && m_Specs.SwapChainTarget)
		{
			m_Specs.Width = 1920;
			m_Specs.Height = 1080;
		}

		for (auto& attachment : info->Attachments)
		{
			if (!OpenGLUtils::IsDepthFormat(attachment.Format)) {
				m_ColorAttachments.emplace_back(attachment);
			}
			else m_DepthAttachment = attachment;
		}
		if (m_Specs.SwapChainTarget) return;

		Invalidate();
	}
	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		HZR_PROFILE_FUNCTION();
		Ref<OpenGLFrameBuffer> instance = this;
		Renderer::SubmitResourceFree([instance]() mutable {
			HZR_PROFILE_FUNCTION("OpenGLFrameBuffer::~OpenGLFrameBuffer_RT()");
			if (instance->GetSpecification().SwapChainTarget) return;

			glDeleteFramebuffers(1, &instance->m_ID);
			if (instance->m_ColorImages.size() > 0) {
				for (auto& image : instance->m_ColorImages)
					image->Release_RT();
			}

			if (instance->m_DepthImage) {
				instance->m_DepthImage->Release_RT();
			}
			});
	}
	void OpenGLFrameBuffer::Bind_RT()
	{
		HZR_PROFILE_FUNCTION();
		HZR_RENDER_THREAD_ONLY();

		glDepthMask(GL_TRUE);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		if (m_Specs.SwapChainTarget)
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
	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height, bool force)
	{
		HZR_PROFILE_FUNCTION();
		Ref<OpenGLFrameBuffer> instance = this;
		Renderer::Submit([instance, width, height, force]() mutable {
			instance->Resize_RT(width, height, force);
			});
	}
	void OpenGLFrameBuffer::Resize_RT(uint32_t width, uint32_t height, bool force)
	{
		HZR_PROFILE_FUNCTION();
		if (m_Specs.Width == width && m_Specs.Height == height && !force || (width < 0 || height < 0))
			return;

		m_Specs.Width = width;
		m_Specs.Height = height;

		Invalidate_RT();
	}
	void OpenGLFrameBuffer::Invalidate()
	{
		HZR_PROFILE_FUNCTION();
		Ref<OpenGLFrameBuffer> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
			});
	}
	void OpenGLFrameBuffer::Invalidate_RT()
	{
		HZR_PROFILE_FUNCTION();
		HZR_RENDER_THREAD_ONLY();
		if (m_Specs.Height > 8192 || m_Specs.Width > 8192) return;

		if (m_Specs.SwapChainTarget) return;


		if (m_ID)
		{
			glDeleteFramebuffers(1, &m_ID);

			if (m_ColorImages.size() > 0)
			{
				if (m_ColorImages.size() > 0)
				{
					for (auto& image : m_ColorImages)
						image->Release_RT();
				}
			}
			m_ColorImages.clear();

			if (m_DepthImage)
			{
				m_DepthImage->Release_RT();
			}
		}

		glCreateFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		bool multisampled = m_Specs.Samples > 1;

		if (m_ColorAttachments.size() > 0)
		{
			m_ColorImages.resize(m_ColorAttachments.size());

			Image2DCreateInfo imageInfo = {};
			imageInfo.Usage = ImageUsage::Attachment;
			imageInfo.Width = m_Specs.Width;
			imageInfo.Height = m_Specs.Height;
			imageInfo.Mips = m_Specs.Samples;

			for (uint32_t i = 0; i < m_ColorImages.size(); i++)
			{
				imageInfo.DebugName = fmt::format("FBO ({0}) color {1}", m_Specs.DebugName, i);
				imageInfo.Format = m_ColorAttachments[i].Format;
				m_ColorImages[i] = Image2D::Create(&imageInfo).As<OpenGLImage2D>();
			}

			for (uint32_t i = 0; i < (uint32_t)m_ColorImages.size(); i++)
			{
				OpenGLUtils::BindTexture(m_ColorImages[i]->GetID(), multisampled);

				switch (m_ColorAttachments[i].Format) {
				case ImageFormat::RGBA:
					OpenGLUtils::AttachColorTexture(m_ColorImages[i]->GetID(), m_Specs.Samples, GL_RGBA, GL_RGBA8, m_Specs.Width, m_Specs.Height, i); break;
				case ImageFormat::RGBA16F:
					OpenGLUtils::AttachColorTexture(m_ColorImages[i]->GetID(), m_Specs.Samples, GL_RGBA, GL_RGBA16F, m_Specs.Width, m_Specs.Height, i); break;
				case ImageFormat::RGB16F:
					OpenGLUtils::AttachColorTexture(m_ColorImages[i]->GetID(), m_Specs.Samples, GL_RGB, GL_RGB16F, m_Specs.Width, m_Specs.Height, i); break;
				default:
					HZR_ASSERT(false, "Unknown format");
				}
			}
		}
		if (m_DepthAttachment.Format != ImageFormat::None)
		{

			Image2DCreateInfo imageInfo = {};
			imageInfo.DebugName = fmt::format("FBO ({}) depth", m_Specs.DebugName);
			imageInfo.Usage = ImageUsage::Attachment;
			imageInfo.Width = m_Specs.Width;
			imageInfo.Height = m_Specs.Height;
			imageInfo.Mips = m_Specs.Samples;
			imageInfo.Format = m_DepthAttachment.Format;

			m_DepthImage = Image2D::Create(&imageInfo).As<OpenGLImage2D>();
			OpenGLUtils::BindTexture(m_DepthImage->GetID(), imageInfo.Mips > 1);

			switch (m_DepthAttachment.Format)
			{
			case ImageFormat::DEPTH24STENCIL8:
				OpenGLUtils::AttachDepthTexture(m_DepthImage->GetID(), m_Specs.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specs.Width, m_Specs.Height);
				break;
			default:
				assert(false);
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
