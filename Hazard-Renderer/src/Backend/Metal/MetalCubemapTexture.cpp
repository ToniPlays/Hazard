#include "MetalCubemapTexture.h"

#ifdef HZR_INCLUDE_METAL

#include "MTLUtils.h"

namespace HazardRenderer::Metal
{
    MetalCubemapTexture::MetalCubemapTexture(CubemapTextureCreateInfo* createInfo)
        : m_FilePath(createInfo->FilePath), m_Format(createInfo->Format)
    {
        HZR_PROFILE_FUNCTION();
        m_DebugName = createInfo->DebugName;

        if (createInfo->pCubemapSrc)
        {
            m_Width = createInfo->pCubemapSrc->pCubemap->GetWidth();
            m_Height = createInfo->pCubemapSrc->pCubemap->GetWidth();
        }
        else
        {
            m_Width = createInfo->Width;
            m_Height = createInfo->Height;
        }
        
        auto device = MetalContext::GetMetalDevice();
        
        MTL::TextureSwizzleChannels channels;
        channels.red = MTL::TextureSwizzleRed;
        channels.green = MTL::TextureSwizzleGreen;
        channels.blue = MTL::TextureSwizzleBlue;
        channels.alpha = MTL::TextureSwizzleAlpha;
        
        MTL::TextureDescriptor* descriptor = MTL::TextureDescriptor::alloc()->init();
        descriptor->setTextureType(MTL::TextureTypeCube);
        descriptor->setWidth(m_Width);
        descriptor->setHeight(m_Height);
        descriptor->setDepth(1);
        descriptor->setArrayLength(1);
        descriptor->setMipmapLevelCount(1);
        descriptor->setPixelFormat(ImageFormatToMTLFormat(m_Format));
        descriptor->setSwizzle(channels);
        
        m_MetalTexture = device->GetMetalDevice()->newTexture(descriptor);
        SetDebugLabel(m_MetalTexture, m_DebugName);
        
        CreateSampler();
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