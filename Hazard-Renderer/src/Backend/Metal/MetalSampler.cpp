
#include "MetalSampler.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Renderer.h"
#include "MetalContext.h"
#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::Metal
{
    MetalSampler::MetalSampler(SamplerCreateInfo* createInfo)
    {
        m_Info = *createInfo;
        Invalidate();
    }
    MetalSampler::~MetalSampler()
    {
        Renderer::SubmitResourceFree([sampler = m_Sampler]() mutable {
                
            });
    }
    void MetalSampler::Invalidate()
    {
        Ref<MetalSampler> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            instance->Invalidate_RT();
            });
    }
    void MetalSampler::Invalidate_RT()
    {
        HZR_PROFILE_FUNCTION();
        
        auto device = MetalContext::GetMetalDevice();
        MTL::SamplerDescriptor* descriptor = MTL::SamplerDescriptor::alloc()->init();
        descriptor->setMaxAnisotropy(1.0);
        descriptor->setMagFilter(MTL::SamplerMinMagFilterLinear);
        descriptor->setMinFilter(MTL::SamplerMinMagFilterLinear);
        descriptor->setRAddressMode(MTL::SamplerAddressModeRepeat);
        descriptor->setSAddressMode(MTL::SamplerAddressModeRepeat);
        descriptor->setTAddressMode(MTL::SamplerAddressModeRepeat);
        
        descriptor->setLodMinClamp(0.0);
        descriptor->setLodMaxClamp(100.0);
        descriptor->setBorderColor(MTL::SamplerBorderColorOpaqueWhite);
        
        m_Sampler = device->GetMetalDevice()->newSamplerState(descriptor);
    }
}
#endif
