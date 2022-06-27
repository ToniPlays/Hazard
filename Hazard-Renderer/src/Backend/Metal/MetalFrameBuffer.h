#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/FrameBuffer.h"


namespace HazardRenderer::Metal
{
    class MetalFrameBuffer : public FrameBuffer {
    public:
        MetalFrameBuffer(FrameBufferCreateInfo* info);
        ~MetalFrameBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void Resize(uint32_t width, uint32_t height, bool force = false) override;
        virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const override;

        virtual uint32_t GetWidth() const override { return m_Specs.Width; };
        virtual uint32_t GetHeight() const override { return m_Specs.Height; };
        
        virtual uint32_t GetColorAttachmentCount() override { return (uint32_t)m_ColorAttachments.size(); }

        virtual Ref<Image2D> GetImage(uint32_t index = 0) const override { return nullptr; /*m_ColorImages[0].As<Image2D>();*/ };
        virtual FrameBufferSpecification& GetSpecification() override { return m_Specs; }


    private:
        
        void Invalidate();
        
        FrameBufferSpecification m_Specs;

        std::vector<FrameBufferAttachment> m_ColorAttachments;
        //std::vector<Ref<MetalImage2D>> m_ColorImages;
        FrameBufferAttachment m_DepthAttachment;
        //Ref<MetalImage2D> m_DepthImage;
    };
}
#endif
