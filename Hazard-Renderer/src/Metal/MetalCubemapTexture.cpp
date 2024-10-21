#include "MetalCubemapTexture.h"

#ifdef HZR_INCLUDE_METAL

#include "MathCore.h"
#include "MTLUtils.h"
#include "Core/Renderer.h"

namespace HazardRenderer::Metal
{
    MetalCubemap::MetalCubemap(CubemapCreateInfo* createInfo)
        : m_Format(createInfo->Format)
    {
        HZR_PROFILE_FUNCTION();
        m_DebugName = createInfo->DebugName;
        m_Usage = createInfo->Usage;
            
        m_Extent = { createInfo->Size, createInfo->Size, 6 };
        
        m_MipLevels = createInfo->MaxMips <= 1 ? 1 : glm::min(Math::GetBaseLog(m_Extent.Width), createInfo->MaxMips);
        
        Ref<MetalCubemap> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            auto device = MetalContext::GetMetalDevice();
            
            MTL::TextureSwizzleChannels channels;
            channels.red = MTL::TextureSwizzleRed;
            channels.green = MTL::TextureSwizzleGreen;
            channels.blue = MTL::TextureSwizzleBlue;
            channels.alpha = MTL::TextureSwizzleAlpha;
            
            MTL::TextureDescriptor* descriptor = MTL::TextureDescriptor::alloc()->init();
            descriptor->setTextureType(MTL::TextureTypeCube);
            descriptor->setWidth(instance->m_Extent.Width);
            descriptor->setHeight(instance->m_Extent.Height);
            descriptor->setDepth(1);
            descriptor->setArrayLength(1);
            descriptor->setMipmapLevelCount(instance->m_MipLevels);
            descriptor->setPixelFormat(ImageFormatToMTLFormat(instance->m_Format));
            descriptor->setSwizzle(channels);
            
            auto usage = instance->m_Usage;
            
            if(usage == ImageUsage::Texture)
                descriptor->setUsage(MTL::TextureUsageShaderRead + MTL::TextureUsageShaderWrite);
            else if(usage == ImageUsage::Storage)
                descriptor->setUsage(MTL::TextureUsageShaderRead + MTL::TextureUsageShaderWrite);
            else if(usage == ImageUsage::Attachment)
                descriptor->setUsage(MTL::TextureUsageShaderRead + MTL::TextureUsageShaderWrite);
            
            instance->m_MetalTexture = device->GetMetalDevice()->newTexture(descriptor);
            SetDebugLabel(instance->m_MetalTexture, instance->m_DebugName);
            
            descriptor->release();
        });
        
        if(m_MipLevels > 1 && false)
            RegenerateMips();
    }

    MetalCubemap::~MetalCubemap()
    {
        Release();
    }

    void MetalCubemap::RegenerateMips()
    {
        Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::Create("Image gen mip", DeviceQueue::TransferBit, 1);

        cmdBuffer->Begin();
        {
            ImageMemoryInfo barrier = {};
            barrier.Image = (Image*)this;
            barrier.BaseLayer = 0;
            barrier.LayerCount = 6;
            barrier.BaseMip = 0;
            barrier.MipCount = 1;
            barrier.SrcLayout = IMAGE_LAYOUT_GENERAL;
            barrier.DstLayout = IMAGE_LAYOUT_TRANSFER_SRC;

            cmdBuffer->ImageMemoryBarrier(barrier);
        }
        {
            ImageMemoryInfo barrier = {};
            barrier.Image = (Image*)this;
            barrier.BaseLayer = 0;
            barrier.LayerCount = 6;
            barrier.BaseMip = 1;
            barrier.MipCount = m_MipLevels - 1;
            barrier.SrcLayout = IMAGE_LAYOUT_UNDEFINED;
            barrier.DstLayout = IMAGE_LAYOUT_TRANSFER_DST;

            cmdBuffer->ImageMemoryBarrier(barrier);
        }

        for (uint32_t face = 0; face < 6; face++)
        {
            for (uint32_t mip = 1; mip < m_MipLevels; mip++)
            {
                BlitImageInfo blit = {};
                blit.Image = (Image*)this;
                blit.SrcExtent = { m_Extent.Width >> (mip - 1), m_Extent.Height >> (mip - 1), 1 };
                blit.SrcLayer = face;
                blit.SrcMip = mip - 1;
                blit.SrcLayout = IMAGE_LAYOUT_TRANSFER_SRC;

                blit.DstExtent = { m_Extent.Width >> mip, m_Extent.Height >> mip, 1 };
                blit.DstLayer = face;
                blit.DstMip = mip;
                blit.DstLayout = IMAGE_LAYOUT_TRANSFER_DST;

                cmdBuffer->BlitImage(blit);

                ImageMemoryInfo barrier = {};
                barrier.Image = (Image*)this;
                barrier.BaseLayer = face;
                barrier.LayerCount = 1;
                barrier.BaseMip = mip;
                barrier.MipCount = 1;
                barrier.SrcLayout = IMAGE_LAYOUT_TRANSFER_DST;
                barrier.DstLayout = IMAGE_LAYOUT_TRANSFER_SRC;

                cmdBuffer->ImageMemoryBarrier(barrier);
            }
        }

        ImageMemoryInfo barrier = {};
        barrier.Image = (Image*)this;
        barrier.BaseLayer = 0;
        barrier.LayerCount = 6;
        barrier.BaseMip = 0;
        barrier.MipCount = m_MipLevels;
        barrier.SrcLayout = IMAGE_LAYOUT_TRANSFER_SRC;
        barrier.DstLayout = IMAGE_LAYOUT_GENERAL;

        cmdBuffer->ImageMemoryBarrier(barrier);

        cmdBuffer->End();
        cmdBuffer->Submit();
    }
    void MetalCubemap::Invalidate()
    {
        
    }

    void MetalCubemap::Release() {
        
    }
}
#endif
