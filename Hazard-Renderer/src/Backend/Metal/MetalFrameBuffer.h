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

        void Resize(uint32_t width, uint32_t height, bool force = false) override {};
        void Resize_RT(uint32_t width, uint32_t height, bool force = false) override {};

        void Release() {};
        void Release_RT() {};
        void Invalidate() {};
        void Invalidate_RT() {};

        uint32_t GetWidth() const override { return m_Specs.Width; };
        uint32_t GetHeight() const override { return m_Specs.Height; };
        size_t GetColorAttachmentCount() override { return m_ColorAttachments.size(); }

        Ref<Image2D> GetImage(uint32_t index = 0) const override { return m_ColorAttachments[index]; };
        Ref<Image2D> GetDepthImage() const override { return m_DepthAttachmentImage; }
        FrameBufferSpecification& GetSpecification() override { return m_Specs; }


    private:
        FrameBufferSpecification m_Specs;
        std::vector<Ref<Image2D>> m_ColorAttachments;
        Ref<Image2D> m_DepthAttachmentImage;
    };
}
#endif
