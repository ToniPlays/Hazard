#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/FrameBuffer.h"
#include "Texture/MetalImage2D.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include<QuartzCore/QuartzCore.hpp>


namespace HazardRenderer::Metal
{
    class MetalFrameBuffer : public FrameBuffer {
    public:
        MetalFrameBuffer(FrameBufferCreateInfo* info);
        ~MetalFrameBuffer();

        void Resize(uint32_t width, uint32_t height, bool force = false) override;
        void Resize_RT(uint32_t width, uint32_t height, bool force = false) override;

        void Release();
        void Release_RT();
        void Invalidate();
        void Invalidate_RT();

        uint32_t GetWidth() const override { return m_Specs.Width; };
        uint32_t GetHeight() const override { return m_Specs.Height; };
        size_t GetColorAttachmentCount() override { return m_ColorAttachments.size(); }

        Ref<Image2D> GetImage(uint32_t index = 0) const override { return m_ColorAttachments[index]; };
        Ref<Image2D> GetDepthImage() const override { return m_DepthAttachmentImage; }
        FrameBufferSpecification& GetSpecification() override { return m_Specs; }
        
        //Metal specific
        MTL::RenderPassDescriptor* GetMetalRenderPassDescriptor() const { return m_RenderPassDescriptor; }

    private:
        FrameBufferSpecification m_Specs;
        std::vector<Ref<MetalImage2D>> m_ColorAttachments;
        std::map<uint32_t, Ref<MetalImage2D>> m_ExistingImages;
        Ref<MetalImage2D> m_DepthAttachmentImage;
        Ref<MetalImage2D> m_ExistingImage;
        
        MTL::RenderPassDescriptor* m_RenderPassDescriptor;
        std::vector<MTL::ClearColor> m_ClearValues;
    };
}
#endif
