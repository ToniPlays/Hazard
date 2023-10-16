#include "MetalDescriptorSet.h"

#ifdef HZR_INCLUDE_METAL
#include "MetalGPUBuffer.h"
#include "MetalGPUBuffer.h"
#include "MetalImage2D.h"
#include "MetalCubemapTexture.h"
#include "MetalTopLevelAS.h"

#include "MetalShader.h"

#include <spdlog/fmt/fmt.h>
#include <glad/glad.h>

namespace HazardRenderer::Metal
{
    MetalDescriptorSet::MetalDescriptorSet(DescriptorSetCreateInfo* info)
    {
        
    }

    void MetalDescriptorSet::Write(uint32_t binding, uint32_t index, Ref<Image> image, Ref<Sampler> sampler, bool updateAll)
    {
        
    };
    void MetalDescriptorSet::Write(uint32_t binding, Ref<GPUBuffer> buffer, bool updateAll)
    {
    
};

    
    void MetalDescriptorSet::BindGraphicsResources(MTL::RenderCommandEncoder* encoder)
    {
        HZR_PROFILE_FUNCTION();
        
    }
    void MetalDescriptorSet::BindComputeResources(MTL::ComputeCommandEncoder *encoder)
    {
        HZR_PROFILE_FUNCTION();
        
        
    }
}
#endif
