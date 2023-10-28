
#include "MetalImage2D.h"
#ifdef HZR_INCLUDE_METAL

#include "Core/Renderer.h"
#include "MetalContext.h"
#include "MTLUtils.h"

namespace HazardRenderer::Metal
{
	MetalImage2D::MetalImage2D(Image2DCreateInfo* info)
    {
        HZR_PROFILE_FUNCTION();
        HZR_ASSERT(!info->DebugName.empty(), "Debug name required");
        HZR_ASSERT(info->Format != ImageFormat::None, "Image format cannot be none");
        HZR_ASSERT(info->Usage != ImageUsage::None, "Image usage cannot be none");

        m_DebugName = info->DebugName;
        m_Width = info->Extent.Width;
        m_Height = info->Extent.Height;
        m_Format = info->Format;
        m_MipLevels = 1; //TODO: Make this work
        m_Usage = info->Usage;
        
        Invalidate();
        
        if(!info->Data) return;
        
        m_LocalBuffer = Buffer::Copy(info->Data.Data, info->Data.Size);
        
        Ref<MetalImage2D> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            instance->UploadImageData_RT();
        });
        
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
        HZR_ASSERT(m_Width > 0 && m_Height > 0, "Image dimensions failed");
        
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
        
    }
    void MetalImage2D::Resize_RT(uint32_t width, uint32_t height)
    {
        
    }
    Buffer MetalImage2D::ReadPixels(const ImageCopyRegion& region)
    {
        Buffer buffer;
        buffer.Allocate(4 * region.Extent.Width * region.Extent.Height * region.Extent.Depth);
        
        Ref<MetalImage2D> instance = this;
        Renderer::Submit([instance, dataBuffer = buffer, region]() mutable {
            
            auto device = MetalContext::GetMetalDevice();
            auto commandBuffer = device->GetGraphicsQueue()->commandBuffer();
            
            MTL::BlitCommandEncoder* encoder = commandBuffer->blitCommandEncoder();
            
            MTL::Buffer* buffer = device->GetMetalDevice()->newBuffer(dataBuffer.Size, MTL::ResourceOptionCPUCacheModeDefault);
        
            encoder->copyFromTexture(instance->m_MetalTexture, 0, 0, { region.X, region.Y, region.Z }, { region.Extent.Width, region.Extent.Height, region.Extent.Depth }, buffer, 0, 4 * region.Extent.Width, 0);
            
            encoder->endEncoding();
            commandBuffer->commit();
            commandBuffer->waitUntilCompleted();
            
            encoder->release();
            commandBuffer->release();
            buffer->release();
            
            dataBuffer.Write(buffer->contents(), dataBuffer.Size);
        });
        return buffer;
    }
    
    void MetalImage2D::Invalidate_RT()
    {
        HZR_PROFILE_FUNCTION();
        HZR_ASSERT(m_Width > 0 && m_Height > 0, "Image dimensions failed");
        
        auto device = MetalContext::GetMetalDevice();
        
        Release_RT();
        
        MTL::TextureDescriptor* descriptor = MTL::TextureDescriptor::alloc()->init();
        descriptor->setWidth(m_Width);
        descriptor->setHeight(m_Height);
        descriptor->setDepth(1);
        descriptor->setSampleCount(1);
        descriptor->setArrayLength(1);
        descriptor->setMipmapLevelCount(m_MipLevels);
        descriptor->setStorageMode(MTL::StorageModeShared);
        descriptor->setPixelFormat(ImageFormatToMTLFormat(m_Format));
        
        if(m_Usage == ImageUsage::Texture)
            descriptor->setUsage(MTL::TextureUsageShaderRead);
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
    void MetalImage2D::UploadImageData_RT()
    {
        MTL::Region region;
        region.size.width = m_Width;
        region.size.height = m_Height;
        region.size.depth = 1;
        region.origin.x = 0;
        region.origin.y = 0;
        region.origin.z = 0;
        
        m_MetalTexture->replaceRegion(region, 0, m_LocalBuffer.Data, 4 * m_Width);
        
        m_LocalBuffer.Release();
    }
}
#endif
