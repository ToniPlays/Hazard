#include "MetalDescriptorSet.h"

#ifdef HZR_INCLUDE_METAL
#include "MetalBuffers.h"
#include "MetalImage2D.h"
#include "MetalCubemapTexture.h"

#include "MetalShader.h"

#include <glad/glad.h>

#ifdef HZR_INCLUDE_METAL

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
    void MetalDescriptorSet::BindResources(MTL::RenderCommandEncoder* encoder, bool isCompute)
    {
        HZR_PROFILE_FUNCTION();
        if (isCompute)
        {
            for (auto& [binding, descriptor] : m_WriteDescriptors)
            {
                switch (descriptor.Type)
                {
                case MTL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                    for (auto& [index, value] : descriptor.BoundValue)
                    {
                        if (!value) continue;
                        auto texture = value.As<MetalImage2D>()->GetMetalTexture();
                        encoder->setFragmentTexture(texture, index);
                    }
                    break;
                case MTL_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                    for (auto& [index, value] : descriptor.BoundValue)
                    {
                        if (descriptor.Dimension == 3)
                        {

                        }
                        else
                        {

                        }
                    }
                    break;
                default: break;
                }
                
            }
            return;
        }
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
    void MetalDescriptorSet::UpdateBindings(const std::unordered_map<std::string, uint32_t> bindings)
    {
        for(auto& [name, binding] : bindings)
        {
            auto writeDescriptor = GetWriteDescriptor(name);
            if(!writeDescriptor) continue;
            
            writeDescriptor->ActualBinding = binding;
            
            std::cout << name << " -> " << binding << std::endl;
        }
    }
}
#endif

#endif