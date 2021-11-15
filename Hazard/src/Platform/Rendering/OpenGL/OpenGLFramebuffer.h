#pragma once

#include "Hazard/Rendering/Pipeline/FrameBuffer.h"
#include "OpenGLImage2D.h"

namespace Hazard::Rendering::OpenGL 
{
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(FrameBufferCreateInfo* info);
		~OpenGLFrameBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual void Resize(uint32_t width, uint32_t height, bool force = false);
		virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const {};

		virtual uint32_t GetWidth() const { return m_Specs.Width; };
		virtual uint32_t GetHeight() const { return m_Specs.Height; };

		virtual Ref<Image2D> GetImage() const { return m_ColorImages[0].As<Image2D>(); };
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