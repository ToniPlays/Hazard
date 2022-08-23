#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL

#include "Backend/Core/FrameBuffer.h"
#include "Textures/OpenGLImage2D.h"

namespace HazardRenderer::OpenGL
{
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(FrameBufferCreateInfo* info);
		~OpenGLFrameBuffer();

		virtual void Bind() override;
		virtual void Bind_RT();
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height, bool force = false) override;
		virtual void BindTexture(uint32_t attachmentIndex = 0, size_t slot = 0) const override {};

		virtual uint32_t GetWidth() const override { return m_Specs.Width; };
		virtual uint32_t GetHeight() const override { return m_Specs.Height; };
		size_t GetColorAttachmentCount() override { return m_ColorAttachments.size(); }

		virtual Ref<Image2D> GetImage(uint32_t index = 0) const override { return m_ColorImages[index].As<Image2D>(); };
		virtual FrameBufferSpecification& GetSpecification() override { return m_Specs; }

		void Invalidate();
		void RT_Invalidate();

	private:
		FrameBufferSpecification m_Specs;
		uint32_t m_ID = 0;

		std::vector<FrameBufferAttachment> m_ColorAttachments;
		std::vector<Ref<OpenGLImage2D>> m_ColorImages;
		FrameBufferAttachment m_DepthAttachment;
		Ref<OpenGLImage2D> m_DepthImage;
	};
}
#endif
