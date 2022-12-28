#include "MetalDescriptorSet.h"

#ifdef HZR_INCLUDE_METAL
#include "MetalBuffers.h"
#include "MetalImage2D.h"
#include "MetalCubemapTexture.h"

#include "MetalShader.h"

#include <glad/glad.h>

namespace HazardRenderer::Metal
{
    void MetalDescriptorSet::AddWriteDescriptor(MetalWriteDescriptor writeDescriptor)
    {
        m_WriteDescriptors[writeDescriptor.Binding] = writeDescriptor;
    }
    MetalWriteDescriptor* MetalDescriptorSet::GetWriteDescriptor(const std::string& name)
    {
        for (auto& [binding, descriptor] : m_WriteDescriptors)
        {
            if (descriptor.DebugName == name)
                return &descriptor;
        }
        return nullptr;
    }
    void MetalDescriptorSet::BindGraphicsResources(MTL::RenderCommandEncoder* encoder)
    {
        HZR_PROFILE_FUNCTION();
        for (auto& [binding, descriptor] : m_WriteDescriptors)
        {
            switch (descriptor.Type)
            {
            case MTL_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
            {
                auto buffer = descriptor.BoundValue[0].As<MetalUniformBuffer>();
                uint32_t flags = buffer->GetUsageFlags();
                
                if(flags & (uint32_t)ShaderStage::Vertex || true)
                {
                    encoder->setVertexBuffer(buffer->GetMetalBuffer(), 0, descriptor.ActualBinding);
                }
                if(flags & (uint32_t)ShaderStage::Fragment || true)
                {
                    encoder->setFragmentBuffer(buffer->GetMetalBuffer(), 0, descriptor.ActualBinding);
                }
                break;
            }
            case MTL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                if(descriptor.Dimension == 3)
                {
                    for (auto& [index, value] : descriptor.BoundValue)
                    {
                        if(index >= 16) break; //TODO: what this
                        
                        auto texture = value.As<MetalCubemapTexture>();
                        encoder->setFragmentTexture(texture->GetMetalTexture(), descriptor.Binding + index);
                        encoder->setFragmentSamplerState(texture->GetMetalSamplerState(), descriptor.Binding + index);
                    }
                }
                else
                {
                    for (auto& [index, value] : descriptor.BoundValue)
                    {
                        if(index >= 16) break; //TODO: what this
                        
                        auto texture = value.As<MetalImage2D>();
                        encoder->setFragmentTexture(texture->GetMetalTexture(), descriptor.Binding + index);
                        encoder->setFragmentSamplerState(texture->GetMetalSamplerState(), descriptor.Binding + index);
                    }
                }
                
                break;
                default: break;
            }
        }
    }
    void MetalDescriptorSet::BindComputeResources(MTL::ComputeCommandEncoder *encoder)
    {
        HZR_PROFILE_FUNCTION();
        for (auto& [binding, descriptor] : m_WriteDescriptors)
        {
            switch (descriptor.Type)
            {
            case MTL_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
            {
                
            }
            case MTL_DESCRIPTOR_TYPE_STORAGE_IMAGE:
            case MTL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                if(descriptor.Dimension == 3)
                {
                    for (auto& [index, value] : descriptor.BoundValue)
                    {
                        if(index >= 16) break; //TODO: what this
                        
                        auto texture = value.As<MetalCubemapTexture>();
                        HZR_ASSERT(texture->GetMetalTexture(), "Texture is nullptr");
                        encoder->setTexture(texture->GetMetalTexture(), descriptor.Binding + index);
                        encoder->setSamplerState(texture->GetMetalSamplerState(), descriptor.Binding + index);
                    }
                }
                else
                {
                    for (auto& [index, value] : descriptor.BoundValue)
                    {
                        if(index >= 16) break; //TODO: what this
                        
                        auto texture = value.As<MetalImage2D>();
                        encoder->setTexture(texture->GetMetalTexture(), descriptor.Binding + index);
                        encoder->setSamplerState(texture->GetMetalSamplerState(), descriptor.Binding + index);
                    }
                }
                break;
                default: break;
            }
        }
    }

    void MetalDescriptorSet::UpdateBindings(const std::unordered_map<std::string, uint32_t> bindings)
    {
        for(auto& [name, binding] : bindings)
        {
            auto writeDescriptor = GetWriteDescriptor(name);
            if(!writeDescriptor) continue;
            
            writeDescriptor->ActualBinding = binding;
        }
    }
}
#endif
