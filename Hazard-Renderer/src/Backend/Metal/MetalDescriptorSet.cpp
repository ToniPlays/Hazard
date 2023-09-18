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
    void MetalDescriptorSet::AddWriteDescriptor(MetalWriteDescriptor writeDescriptor)
    {
        auto& descriptor = m_WriteDescriptors[writeDescriptor.Type];
        HZR_ASSERT(descriptor.find(writeDescriptor.Binding) == descriptor.end(), "");
        descriptor[writeDescriptor.Binding] = writeDescriptor;
    }
    MetalWriteDescriptor* MetalDescriptorSet::GetWriteDescriptor(uint32_t type, const std::string& name)
    {
        for (auto& [binding, descriptor] : m_WriteDescriptors[type])
        {
            if (descriptor.DebugName == name)
                return &descriptor;
        }
        return nullptr;
    }
    void MetalDescriptorSet::BindGraphicsResources(MTL::RenderCommandEncoder* encoder)
    {
        HZR_PROFILE_FUNCTION();
        
        for(auto& [type, descriptors] : m_WriteDescriptors)
        {
            for (auto& [binding, descriptor] : descriptors)
            {
                switch (descriptor.Type)
                {
                    case MTL_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                    {
                        /*
                        auto buffer = descriptor.BoundValue[0].As<MetalUniformBuffer>();
                        uint32_t flags = descriptor.Flags;
                        
                        if(flags & (uint32_t)ShaderStage::Vertex)
                            encoder->setVertexBuffer(buffer->GetMetalBuffer(), 0, descriptor.ActualBinding);
                        
                        if(flags & (uint32_t)ShaderStage::Fragment)
                            encoder->setFragmentBuffer(buffer->GetMetalBuffer(), 0, descriptor.ActualBinding);
                         */
                        break;
                    }
                    case MTL_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                    {
                        /*auto buffer = descriptor.BoundValue[0].As<MetalArgumentBuffer>();
                        uint32_t flags = descriptor.Flags;
                        
                        if(flags & (uint32_t)ShaderStage::Vertex)
                            encoder->setVertexBuffer(buffer->GetMetalBuffer(), 0, descriptor.ActualBinding);
                        
                        if(flags & (uint32_t)ShaderStage::Fragment)
                            encoder->setFragmentBuffer(buffer->GetMetalBuffer(), 0, descriptor.ActualBinding);
                         */
                        break;
                    }
                    case MTL_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                    case MTL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                        if(descriptor.Dimension == 3)
                        {
                            for (auto& [index, value] : descriptor.BoundValue)
                            {
                                if(index >= 16) break; //TODO: what this
                                
                                auto texture = value.As<MetalCubemapTexture>();
                                encoder->setFragmentTexture(texture->GetMetalTexture(), descriptor.ActualBinding + index);
                                encoder->setFragmentSamplerState(texture->GetMetalSamplerState(), descriptor.SecondaryBinding + index);
                            }
                            break;
                        }
                        else
                        {
                            for (auto& [index, value] : descriptor.BoundValue)
                            {
                                if(index >= 16) break; //TODO: what this
                                
                                auto texture = value.As<MetalImage2D>();
                                encoder->setFragmentTexture(texture->GetMetalTexture(), descriptor.ActualBinding + index);
                                encoder->setFragmentSamplerState(texture->GetMetalSamplerState(), descriptor.SecondaryBinding + index);
                            }
                            break;
                        }
                    case MTL_DESCRIPTOR_TYPE_PUSH_CONSTANT:
                    {
                        void* data = descriptor.Buffer.Data;
                        uint32_t flags = descriptor.Flags;
                        
                        /*
                        if(flags & (uint32_t)ShaderStage::Vertex)
                            encoder->setVertexBytes(data, descriptor.Size, descriptor.ActualBinding);
                        
                        if(flags & (uint32_t)ShaderStage::Fragment)
                            encoder->setFragmentBytes(data, descriptor.Size, descriptor.ActualBinding);
                         */
                        break;
                    }
                    default: break;
                }
            }
        }
    }
    void MetalDescriptorSet::BindComputeResources(MTL::ComputeCommandEncoder *encoder)
    {
        HZR_PROFILE_FUNCTION();
        
        for(auto& [type, descriptors] : m_WriteDescriptors)
        {
            for (auto& [binding, descriptor] : descriptors)
            {
                switch (descriptor.Type)
                {
                    case MTL_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                    {
                        /*
                        auto buffer = descriptor.BoundValue[0].As<MetalUniformBuffer>();
                        encoder->setBuffer(buffer->GetMetalBuffer(), 0, descriptor.ActualBinding);
                         */
                        break;
                    }
                    case MTL_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                    {
                        /*
                        auto buffer = descriptor.BoundValue[0].As<MetalArgumentBuffer>();
                        encoder->setBuffer(buffer->GetMetalBuffer(), 0, descriptor.ActualBinding);
                         */
                        break;
                    }
                    case MTL_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                        if(descriptor.Dimension == 3)
                        {
                            for (auto& [index, value] : descriptor.BoundValue)
                            {
                                if(index >= 16) break; //TODO: what this
                                
                                auto texture = value.As<MetalCubemapTexture>();
                                encoder->setTexture(texture->GetMetalTexture(), descriptor.ActualBinding + index);
                            }
                            break;
                        }
                        else
                        {
                            for (auto& [index, value] : descriptor.BoundValue)
                            {
                                if(index >= 16) break; //TODO: what this
                                
                                auto texture = value.As<MetalImage2D>();
                                encoder->setTexture(texture->GetMetalTexture(), descriptor.ActualBinding + index);
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
                                encoder->setTexture(texture->GetMetalTexture(), descriptor.ActualBinding + index);
                                encoder->setSamplerState(texture->GetMetalSamplerState(), descriptor.SecondaryBinding + index);
                            }
                            break;
                        }
                        else
                        {
                            for (auto& [index, value] : descriptor.BoundValue)
                            {
                                if(index >= 16) break; //TODO: what this
                                
                                auto texture = value.As<MetalImage2D>();
                                encoder->setTexture(texture->GetMetalTexture(), descriptor.ActualBinding + index);
                                encoder->setSamplerState(texture->GetMetalSamplerState(), descriptor.SecondaryBinding + index);
                            }
                            break;
                        }
                    case MTL_DESCRIPTOR_TYPE_PUSH_CONSTANT:
                    {
                        encoder->setBytes(descriptor.Buffer.Data, descriptor.Size, descriptor.ActualBinding);
                        break;
                    }
                    default: break;
                }
            }
        }
    }

    void MetalDescriptorSet::UpdateBindings(const std::unordered_map<std::string, MSLBinding> bindings)
    {
        for(auto& [name, binding] : bindings)
        {
            for(auto& [type, descriptor] : m_WriteDescriptors)
            {
                auto writeDescriptor = GetWriteDescriptor(type, name);
                if(!writeDescriptor) continue;
                
                
                writeDescriptor->ActualBinding = binding.Binding;
                writeDescriptor->SecondaryBinding = binding.SamplerBinding;
                
                break;
            }
        }
    }
}
#endif
