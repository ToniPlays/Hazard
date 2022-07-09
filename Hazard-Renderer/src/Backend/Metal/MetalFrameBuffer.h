#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/FrameBuffer.h"

namespace MTL {
class RenderPassDescriptor;
}

namespace HazardRenderer::Metal
{
    class MetalFrameBuffer : public FrameBuffer {
    public:
        MetalFrameBuffer(FrameBufferCreateInfo* info);
        ~MetalFrameBuffer();

        void Bind() override;
        void Unbind() override;

        void Resize(uint32_t width, uint32_t height, bool force = false) override;
        void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const override;

        virtual uint32_t GetWidth() const override { return m_Specs.Width; };
        virtual uint32_t GetHeight() const override { return m_Specs.Height; };
        
        virtual uint32_t GetColorAttachmentCount() override { return (uint32_t)m_ColorAttachments.size(); }

        virtual Ref<Image2D> GetImage(uint32_t index = 0) const override { return nullptr; /*m_ColorImages[0].As<Image2D>();*/ };
        virtual FrameBufferSpecification& GetSpecification() override { return m_Specs; }
        
        virtual MTL::RenderPassDescriptor* GetMetalRenderPassDescriptor() { return m_RenderPassDescriptor; }

    private:
        
        void Invalidate();
        
        FrameBufferSpecification m_Specs;
        MTL::RenderPassDescriptor* m_RenderPassDescriptor;

        std::vector<FrameBufferAttachment> m_ColorAttachments;
        //std::vector<Ref<MetalImage2D>> m_ColorImages;
        FrameBufferAttachment m_DepthAttachment;
        //Ref<MetalImage2D> m_DepthImage;
    };
}
#endif
