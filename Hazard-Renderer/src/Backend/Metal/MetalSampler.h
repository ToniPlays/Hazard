
#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Sampler.h"
#include <Metal/Metal.hpp>

namespace HazardRenderer::Metal
{
    class MetalSampler : public Sampler
    {
    public:
        MetalSampler(SamplerCreateInfo* createInfo);
        ~MetalSampler();

        const std::string& GetDebugName() const { return m_Info.DebugName; };
        FilterMode GetMinFilter() const { return m_Info.MinFilter; };
        FilterMode GetMagFilter() const { return m_Info.MagFilter; };
        ImageWrap GetWrapping() const { return m_Info.Wrapping; };
        
        //Metal specific
        void Invalidate();
        void Invalidate_RT();

        MTL::SamplerState* GetMetalSampler() const { return m_Sampler; }

    private:
        SamplerCreateInfo m_Info;
        MTL::SamplerState* m_Sampler;
    };
}
#endif
