#include "MetalDescriptorSet.h"

#ifdef HZR_INCLUDE_METAL
#include "MetalGPUBuffer.h"
#include "MetalGPUBuffer.h"
#include "MetalImage2D.h"
#include "MetalCubemapTexture.h"
#include "MetalTopLevelAS.h"
#include "Backend/Core/Renderer.h"

#include "MetalShader.h"
#include "MetalSampler.h"

#include <spdlog/fmt/fmt.h>
#include <glad/glad.h>

namespace HazardRenderer::Metal
{
    MetalDescriptorSet::MetalDescriptorSet(DescriptorSetCreateInfo* info)
    {
        m_DebugName = info->DebugName;
        m_Set = info->Set;
        m_Layout = *info->pLayout;
        
        Invalidate();
    }

    void MetalDescriptorSet::Invalidate()
    {
        Ref<MetalDescriptorSet> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            instance->Invalidate_RT();
        });
    }
    
    void MetalDescriptorSet::Invalidate_RT()
    {
        for(auto& element : m_Layout)
        {
            uint32_t baseIndex = element.Binding;
            for(uint32_t i = 0; i < element.Length; i++)
            {
                m_DescriptorValues[baseIndex + i] = { nullptr, nullptr };
            }
        }
    }
    
    void MetalDescriptorSet::Write(uint32_t binding, uint32_t index, Ref<Image> image, Ref<Sampler> sampler, bool updateAll)
    {
        Ref<MetalDescriptorSet> instance = this;
        Ref<MetalImage2D> mtlImage = image.As<MetalImage2D>();
        Ref<MetalSampler> mtlSampler = sampler.As<MetalSampler>();
        
        Renderer::Submit([instance, binding, index, mtlImage, mtlSampler]() mutable {
            instance->m_DescriptorValues[binding + index] = { mtlImage, mtlSampler };
        });
    }
    void MetalDescriptorSet::Write(uint32_t binding, Ref<GPUBuffer> buffer, bool updateAll)
    {
        Renderer::Submit([]() mutable {
            
        });
    }

    void MetalDescriptorSet::BindGraphicsResources(MTL::RenderCommandEncoder* encoder)
    {
        HZR_PROFILE_FUNCTION();
        
        for(auto& element : m_Layout)
        {
            auto& b = m_DescriptorValues[element.Binding];
            
            switch(element.Type)
            {
                case DESCRIPTOR_TYPE_SAMPLER_2D:
                    encoder->setFragmentTexture(b.Value.As<MetalImage2D>()->GetMetalTexture(), element.Binding);
                    encoder->setFragmentSamplerState(b.Secondary.As<MetalSampler>()->GetMetalSampler(), element.Binding);
                    break;
                case DESCRIPTOR_TYPE_SAMPLER_CUBE:
                    
                    break;
                case DESCRIPTOR_TYPE_STORAGE_IMAGE:
                    
                    break;
                case DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                    
                    break;
                case DESCRIPTOR_TYPE_STORAGE_BUFFER:
                    
                    break;
                case DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE:
                    
                    break;
            }
        }
    }
    void MetalDescriptorSet::BindComputeResources(MTL::ComputeCommandEncoder *encoder)
    {
        HZR_PROFILE_FUNCTION();
        
        
    }
}
#endif
