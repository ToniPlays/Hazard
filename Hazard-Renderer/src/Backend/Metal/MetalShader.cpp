
#include "MetalShader.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalShaderCompiler.h"
#include "MetalContext.h"
#include "Renderer.h"

namespace HazardRenderer::Metal
{
    MetalShader::MetalShader(const std::vector<ShaderStageCode>& shaderCode)
    {
        for(auto& stageCode : shaderCode)
            m_ShaderCode[stageCode.Stage] = stageCode.ShaderCode;
        Reload();
    }
    MetalShader::~MetalShader()
    {
        
    }
    void MetalShader::Reload()
    {
        HZR_PROFILE_FUNCTION();
        
        auto device = MetalContext::GetMetalDevice();
        
        MetalShaderCompiler compiler;
        
        for(auto& [stage, spirv] : m_ShaderCode)
        {
            NS::Error* libError;
            std::string result;
            
            if(!compiler.Decompile(spirv, result))
            {
                std::cout << result << std::endl;
                std::cout << compiler.GetErrorMessage() << std::endl;
                break;
            }
            
            NS::String* source = NS::String::alloc()->string(result.c_str(), NS::UTF8StringEncoding);
            MTL::CompileOptions* options = MTL::CompileOptions::alloc()->init();
            
            MTL::Library* lib = device->GetMetalDevice()->newLibrary(source, options, &libError);
            
            NS::Error* functionError;
            std::string shaderTypeName = "main0";
            NS::String* name = NS::String::string()->string(shaderTypeName.c_str(), NS::UTF8StringEncoding);
            MTL::FunctionDescriptor* descriptor = MTL::FunctionDescriptor::functionDescriptor();
                        
            descriptor->setName(name);
            
            m_Functions[stage] = lib->newFunction(descriptor, &functionError);
        }
        
        Reflect();
    }

    void MetalShader::Set(const std::string &name, uint32_t index, Ref<Image2D> image)
    {
        for (auto& [set, descriptor] : m_DescriptorSet)
        {
            auto write = descriptor.GetWriteDescriptor(name);
            if (write->DebugName != name)
                continue;

            write->BoundValue[index] = image;
            return;
        }
    }
    void MetalShader::Set(const std::string &name, uint32_t index, Ref<CubemapTexture> cubemap)
    {
        for (auto& [set, descriptor] : m_DescriptorSet)
        {
            auto* write = descriptor.GetWriteDescriptor(name);
            if (!write)
                continue;

            write->BoundValue[index] = cubemap;
            return;
        }
    }
    void MetalShader::Set(const std::string &name, uint32_t index, Ref<AccelerationStructure> accelerationStructure)
    {
        
    }
    void MetalShader::Set(const std::string &name, uint32_t index, Ref<BufferBase> buffer)
    {
        
    }
    void MetalShader::BindResources(MTL::RenderCommandEncoder* encoder)
    {
        for(auto& [set, descriptor] : m_DescriptorSet)
            descriptor.BindResources(encoder);
    }
    void MetalShader::Reflect()
    {
        m_ShaderData = ShaderCompiler::GetShaderResources(m_ShaderCode);

        for (auto& [set, buffers] : m_ShaderData.UniformsDescriptions)
        {
            MetalDescriptorSet& descriptorSet = m_DescriptorSet[set];
            for (auto& [binding, buffer] : buffers)
            {
                MetalWriteDescriptor writeDescriptor = {};
                writeDescriptor.Type = MTL_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                writeDescriptor.DebugName = buffer.Name;
                writeDescriptor.Binding = binding;
                writeDescriptor.ArraySize = 0;

                descriptorSet.AddWriteDescriptor(writeDescriptor);

                UniformBufferCreateInfo bufferInfo = {};
                bufferInfo.Name = buffer.Name;
                bufferInfo.Set = set;
                bufferInfo.Binding = binding;
                bufferInfo.Size = buffer.Size;
                bufferInfo.Usage = buffer.UsageFlags;
                
                descriptorSet.GetWriteDescriptor(binding)->BoundValue[0] = UniformBuffer::Create(&bufferInfo);
            }
        }
        for (auto& [set, samplers] : m_ShaderData.ImageSamplers)
        {
            MetalDescriptorSet& descriptorSet = m_DescriptorSet[set];
            for (auto& [binding, sampler] : samplers)
            {
                MetalWriteDescriptor writeDescriptor = {};
                writeDescriptor.Type = MTL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                writeDescriptor.DebugName = sampler.Name;
                writeDescriptor.Binding = binding;
                writeDescriptor.ArraySize = sampler.ArraySize;
                writeDescriptor.Dimension = sampler.Dimension;

                descriptorSet.AddWriteDescriptor(writeDescriptor);

                auto& whiteTexture = MetalContext::GetInstance()->GetDefaultResources().WhiteTexture;
                
                for (uint32_t i = 0; i < sampler.ArraySize; i++)
                {
                    if (sampler.Dimension == 2)
                        Set(sampler.Name, i, whiteTexture);
                }
            }
        }
        for (auto& [set, storageImage] : m_ShaderData.StorageImages)
        {
            MetalDescriptorSet& descriptorSet = m_DescriptorSet[set];
            for (auto& [binding, image] : storageImage)
            {
                MetalWriteDescriptor writeDescriptor = {};
                writeDescriptor.Type = MTL_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                writeDescriptor.DebugName = image.Name;
                writeDescriptor.Binding = binding;
                writeDescriptor.ArraySize = image.ArraySize;
                writeDescriptor.ActualBinding = binding;
                writeDescriptor.Dimension = image.Dimension;

                descriptorSet.AddWriteDescriptor(writeDescriptor);
            }
        }
        
        MetalShaderCompiler compiler;
        for(auto& [stage, code] : m_ShaderCode)
        {
            auto resources = compiler.GetMSLBindings(code);
            for(auto& [set, descriptor] : m_DescriptorSet)
                descriptor.UpdateBindings(resources);
        }
    }
}
#endif
