#include "MetalCubemapTexture.h"

#ifdef HZR_INCLUDE_METAL

#include "MTLUtils.h"
#include "Backend/Core/Renderer.h"

namespace HazardRenderer::Metal
{
    MetalCubemapTexture::MetalCubemapTexture(CubemapTextureCreateInfo* createInfo)
        : m_Format(createInfo->Format)
    {
        HZR_PROFILE_FUNCTION();
        m_DebugName = createInfo->DebugName;
        m_Usage = createInfo->Usage;
            
        m_Width = createInfo->Width;
        m_Height = createInfo->Height;
        
        m_MipLevels = createInfo->GenerateMips ? GetMipLevelCount(m_Width, m_Height) : 1;
        
        Ref<MetalCubemapTexture> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            auto device = MetalContext::GetMetalDevice();
            
            MTL::TextureSwizzleChannels channels;
            channels.red = MTL::TextureSwizzleRed;
            channels.green = MTL::TextureSwizzleGreen;
            channels.blue = MTL::TextureSwizzleBlue;
            channels.alpha = MTL::TextureSwizzleAlpha;
            
            MTL::TextureDescriptor* descriptor = MTL::TextureDescriptor::alloc()->init();
            descriptor->setTextureType(MTL::TextureTypeCube);
            descriptor->setWidth(instance->m_Width);
            descriptor->setHeight(instance->m_Height);
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
            
            instance->CreateSampler();
        });
        
        if(createInfo->Data)
            SetImageData(createInfo->Data);
    }
    void MetalCubemapTexture::SetImageData(const Buffer& data)
    {
        m_LocalBuffer = Buffer::Copy(data);
        Ref<MetalCubemapTexture> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            instance->UploadImageData_RT();
        });
    }
    void MetalCubemapTexture::UploadImageData_RT()
    {
        MTL::Region region = {};
        region.origin = { 0, 0, 0 };
        region.size = { m_Width, m_Height, 1 };
        
        for(uint32_t i = 0; i < 6; i++)
            m_MetalTexture->replaceRegion(region, 0, i, m_LocalBuffer.Data, 4 * m_Width, 0);
        
        m_LocalBuffer.Release();
    }

    void MetalCubemapTexture::GenerateMipmaps_RT(MTL::CommandBuffer* commandBuffer)
    {
        auto blitEncoder = commandBuffer->blitCommandEncoder();
        blitEncoder->generateMipmaps(m_MetalTexture);
        blitEncoder->endEncoding();
    }
    
    void MetalCubemapTexture::CreateSampler()
    {
        auto device = MetalContext::GetMetalDevice();
        
        MTL::SamplerDescriptor* descriptor = MTL::SamplerDescriptor::alloc()->init();
        SetDebugLabel(descriptor, m_DebugName);
        
        descriptor->setMaxAnisotropy(1.0f);
        descriptor->setMinFilter(MTL::SamplerMinMagFilterLinear);
        descriptor->setMagFilter(MTL::SamplerMinMagFilterLinear);
        descriptor->setMipFilter(MTL::SamplerMipFilterLinear);
        descriptor->setRAddressMode(MTL::SamplerAddressModeClampToEdge);
        descriptor->setSAddressMode(MTL::SamplerAddressModeClampToEdge);
        descriptor->setTAddressMode(MTL::SamplerAddressModeClampToEdge);
        descriptor->setBorderColor(MTL::SamplerBorderColorOpaqueWhite);
        descriptor->setLodMinClamp(0.0f);
        descriptor->setLodMaxClamp(100.0f);
        
        m_MetalSampler = device->GetMetalDevice()->newSamplerState(descriptor);
    }
}
#endif
