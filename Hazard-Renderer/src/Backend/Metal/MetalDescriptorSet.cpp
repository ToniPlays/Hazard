#include "MetalDescriptorSet.h"

#include "OpenGLBuffers.h"
#include "MetalImage2D.h"

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

                break;
            case MTL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                if (descriptor.Dimension == 3)
                {

                }
                else
                {
                    for (auto& [index, value] : descriptor.BoundValue)
                    {
                        if (!value) continue;
                        auto texture = value.As<MetalImage2D>();
                        encoder->setFragmentTexture(texture->GetMetalTexture(), descriptor.Binding);
                        encoder->setFragmentSamplerState(texture->GetMetalSamplerState(), descriptor.Binding);
                    }
                }
                break;
                default: break;
            }
        }
    }
}
#endif

