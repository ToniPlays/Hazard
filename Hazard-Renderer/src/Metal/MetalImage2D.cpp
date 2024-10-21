
#include "MetalImage2D.h"
#ifdef HZR_INCLUDE_METAL

#include "Core/Renderer.h"
#include "MetalContext.h"
#include "MTLUtils.h"
#include "MathCore.h"

namespace HazardRenderer::Metal
{
	MetalImage2D::MetalImage2D(Image2DCreateInfo* info)
    {
        HZR_PROFILE_FUNCTION();
        HZR_ASSERT(!info->DebugName.empty(), "Debug name required");
        HZR_ASSERT(info->Format != ImageFormat::None, "Image format cannot be none");
        HZR_ASSERT(info->Usage != ImageUsage::None, "Image usage cannot be none");

        m_DebugName = info->DebugName;
        m_Extent = info->Extent;
        m_Format = info->Format;
        m_MipLevels = info->MaxMips <= 1 ? 1 : glm::min(Math::GetBaseLog(info->MaxMips), info->MaxMips);
        m_Usage = info->Usage;
        
        Invalidate();
    
        if (!info->Data.Data) return;

        ImageCopyRegion region = {};
        region.Extent = m_Extent;
        region.X = 0;
        region.Y = 0;
        region.Z = 0;
        region.Data = info->Data.Data;
        region.DataSize = info->Data.Size;

        ImageMemoryInfo barrier = {};
        barrier.Image = (Image*)this;
        barrier.BaseLayer = 0;
        barrier.LayerCount = 1;
        barrier.BaseMip = 0;
        barrier.MipCount = 1;
        barrier.SrcLayout = IMAGE_LAYOUT_UNDEFINED;
        barrier.DstLayout = IMAGE_LAYOUT_TRANSFER_DST;

        Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::Create("ImageUpload", DeviceQueue::TransferBit, 1);
        cmdBuffer->Begin();
        cmdBuffer->ImageMemoryBarrier(barrier);
        cmdBuffer->CopyToImage(this, region);

        barrier.SrcLayout = IMAGE_LAYOUT_TRANSFER_DST;
        barrier.DstLayout = IMAGE_LAYOUT_SHADER_READ_ONLY;
        cmdBuffer->ImageMemoryBarrier(barrier);

        cmdBuffer->End();
        cmdBuffer->Submit();

        if (m_MipLevels > 1)
            GenerateMips();
        
    }
    MetalImage2D::~MetalImage2D()
    {
        Renderer::SubmitResourceFree([buffer = m_LocalBuffer, texture = m_MetalTexture]() mutable {
            buffer.Release();
            texture->release();
        });
    }
    void MetalImage2D::Invalidate()
    {
        HZR_PROFILE_FUNCTION();
        HZR_ASSERT(m_Extent.Width > 0 && m_Extent.Height > 0, "Image dimensions failed");
        
        Ref<MetalImage2D> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            instance->Invalidate_RT();
            });
    }

    void MetalImage2D::Release()
    {
        Ref<MetalImage2D> instance = this;
        Renderer::SubmitResourceFree([instance]() mutable {
            instance->Release_RT();
        });
    }
    void MetalImage2D::Release_RT()
    {
        if(m_MetalTexture)
            m_MetalTexture->release();
    }
    void MetalImage2D::Resize_RT(uint32_t width, uint32_t height)
    {
        
    }
    
    void MetalImage2D::Invalidate_RT()
    {
        HZR_PROFILE_FUNCTION();
        HZR_ASSERT(m_Extent.Width > 0 && m_Extent.Height > 0, "Image dimensions failed");
        
        auto device = MetalContext::GetMetalDevice();
        
        Release_RT();
        
        MTL::TextureDescriptor* descriptor = MTL::TextureDescriptor::alloc()->init();
        descriptor->setWidth(m_Extent.Width);
        descriptor->setHeight(m_Extent.Height);
        descriptor->setDepth(m_Extent.Depth);
        descriptor->setSampleCount(1);
        descriptor->setArrayLength(1);
        descriptor->setMipmapLevelCount(m_MipLevels);
        descriptor->setStorageMode(MTL::StorageModeShared);
        descriptor->setPixelFormat(ImageFormatToMTLFormat(m_Format));
        
        if(m_Usage == ImageUsage::Texture)
            descriptor->setUsage(MTL::TextureUsageShaderRead + MTL::TextureUsageShaderWrite);
        else if(m_Usage == ImageUsage::Storage)
            descriptor->setUsage(MTL::TextureUsageShaderWrite + MTL::TextureUsageShaderRead);
        else
            descriptor->setUsage(MTL::TextureUsageRenderTarget + MTL::TextureUsageShaderRead);
        
        MTL::TextureSwizzleChannels swizzle;
        swizzle.red = MTL::TextureSwizzleRed;
        swizzle.green = MTL::TextureSwizzleGreen;
        swizzle.blue = MTL::TextureSwizzleBlue;
        swizzle.alpha = MTL::TextureSwizzleAlpha;
        descriptor->setSwizzle(swizzle);
        
        m_MetalTexture = device->GetMetalDevice()->newTexture(descriptor);
        
        SetDebugLabel(m_MetalTexture, m_DebugName);
        
        descriptor->release();
    }
    void MetalImage2D::GenerateMips()
    {
        Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::Create("Image gen mip", DeviceQueue::TransferBit, 1);

        cmdBuffer->Begin();
        {
            ImageMemoryInfo barrier = {};
            barrier.Image = (Image*)this;
            barrier.BaseLayer = 0;
            barrier.LayerCount = 1;
            barrier.BaseMip = 0;
            barrier.MipCount = 1;
            barrier.SrcLayout = IMAGE_LAYOUT_SHADER_READ_ONLY;
            barrier.DstLayout = IMAGE_LAYOUT_TRANSFER_SRC;

            cmdBuffer->ImageMemoryBarrier(barrier);
        }
        {
            ImageMemoryInfo barrier = {};
            barrier.Image = (Image*)this;
            barrier.BaseLayer = 0;
            barrier.LayerCount = 1;
            barrier.BaseMip = 1;
            barrier.MipCount = m_MipLevels - 1;
            barrier.SrcLayout = IMAGE_LAYOUT_UNDEFINED;
            barrier.DstLayout = IMAGE_LAYOUT_TRANSFER_DST;

            cmdBuffer->ImageMemoryBarrier(barrier);
        }

        for (uint32_t mip = 1; mip < m_MipLevels; mip++)
        {
            BlitImageInfo blit = {};
            blit.Image = (Image*)this;
            blit.SrcExtent = { m_Extent.Width >> (mip - 1), m_Extent.Height >> (mip - 1), 1 };
            blit.SrcLayer = 0;
            blit.SrcMip = mip - 1;
            blit.SrcLayout = IMAGE_LAYOUT_TRANSFER_SRC;

            blit.DstExtent = { m_Extent.Width >> mip, m_Extent.Height >> mip, 1 };
            blit.DstLayer = 0;
            blit.DstMip = mip;
            blit.DstLayout = IMAGE_LAYOUT_TRANSFER_DST;

            cmdBuffer->BlitImage(blit);

            ImageMemoryInfo barrier = {};
            barrier.Image = (Image*)this;
            barrier.BaseLayer = 0;
            barrier.LayerCount = 1;
            barrier.BaseMip = mip;
            barrier.MipCount = 1;
            barrier.SrcLayout = IMAGE_LAYOUT_TRANSFER_DST;
            barrier.DstLayout = IMAGE_LAYOUT_TRANSFER_SRC;

            cmdBuffer->ImageMemoryBarrier(barrier);
        }

        ImageMemoryInfo barrier = {};
        barrier.Image = (Image*)this;
        barrier.BaseLayer = 0;
        barrier.LayerCount = 1;
        barrier.BaseMip = 0;
        barrier.MipCount = m_MipLevels;
        barrier.SrcLayout = IMAGE_LAYOUT_TRANSFER_SRC;
        barrier.DstLayout = IMAGE_LAYOUT_SHADER_READ_ONLY;

        cmdBuffer->ImageMemoryBarrier(barrier);

        cmdBuffer->End();
        cmdBuffer->Submit();
    }
}
#endif
