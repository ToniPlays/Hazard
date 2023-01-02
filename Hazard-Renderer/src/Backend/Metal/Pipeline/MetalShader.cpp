
#include "MetalShader.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalShaderCompiler.h"
#include "MetalContext.h"
#include "Renderer.h"
#include "MetalVertexBuffer.h"

#include "Window.h"

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
            
            if(!compiler.Decompile(spirv, result, stage == ShaderStage::Raygen))
            {
                std::cout << result << std::endl;
                std::cout << compiler.GetErrorMessage() << std::endl;
                break;
            }
            
            NS::String* source = NS::String::alloc()->string(result.c_str(), NS::UTF8StringEncoding);
            MTL::CompileOptions* options = MTL::CompileOptions::alloc()->init();
            options->setFastMathEnabled(true);
            options->setPreserveInvariance(true);
            options->setLanguageVersion(MTL::LanguageVersion2_4);
            
            MTL::Library* lib = device->GetMetalDevice()->newLibrary(source, options, &libError);
            
            if(libError->code() != 0)
            {
                RenderMessage message = {};
                message.Severity = Severity::Error;
                message.Description = libError->description()->utf8String();
                message.StackTrace = result;
                
                Window::SendDebugMessage(message);
                continue;
            }
            
            NS::Error* functionError = NS::Error::alloc();
            std::string shaderTypeName = "main0";
            NS::String* name = NS::String::string()->string(shaderTypeName.c_str(), NS::UTF8StringEncoding);
            
            MTL::FunctionDescriptor* descriptor = MTL::FunctionDescriptor::functionDescriptor();
            descriptor->setName(name);
            
            auto func = lib->newFunction(descriptor, &functionError);
            
            if(functionError->code() != 0)
            {
                RenderMessage message = {};
                message.Severity = Severity::Error;
                message.Description = functionError->description()->utf8String();
                message.StackTrace = functionError->localizedFailureReason()->utf8String();
                
                Window::SendDebugMessage(message);
                continue;
            }
            
            m_Functions[stage] = func;
        }
        
        Reflect();
    }

    void MetalShader::Set(const std::string &name, uint32_t index, Ref<Image2D> image)
    {
        for (auto& [set, descriptor] : m_DescriptorSet)
        {
            auto write = descriptor.GetWriteDescriptor(MTL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, name);
            if(write == nullptr) continue;
            if (write->DebugName != name)
                continue;
            
            Ref<Image2D> instance = image;
            Renderer::Submit([write, index, instance]() {
                write->BoundValue[index] = instance;
            });
            return;
        }
        for (auto& [set, descriptor] : m_DescriptorSet)
        {
            auto write = descriptor.GetWriteDescriptor(MTL_DESCRIPTOR_TYPE_STORAGE_IMAGE, name);
            if(write == nullptr) continue;
            if (write->DebugName != name)
                continue;
            
            Ref<Image2D> instance = image;
            Renderer::Submit([write, index, instance]() {
                write->BoundValue[index] = instance;
            });
            return;
        }
    }
    void MetalShader::Set(const std::string &name, uint32_t index, Ref<CubemapTexture> cubemap)
    {
        for (auto& [set, descriptor] : m_DescriptorSet)
        {
            auto* write = descriptor.GetWriteDescriptor(MTL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, name);
            if (!write) continue;

            Ref<CubemapTexture> instance = cubemap;
            Renderer::Submit([write, index, instance]() {
                write->BoundValue[index] = instance;
            });
            return;
        }
        
        for (auto& [set, descriptor] : m_DescriptorSet)
        {
            auto* write = descriptor.GetWriteDescriptor(MTL_DESCRIPTOR_TYPE_STORAGE_IMAGE, name);
            if (!write) continue;

            Ref<CubemapTexture> instance = cubemap;
            Renderer::Submit([write, index, instance]() {
                write->BoundValue[index] = instance;
            });
            return;
        }
    }
    void MetalShader::Set(const std::string &name, uint32_t index, Ref<AccelerationStructure> accelerationStructure)
    {
        for (auto& [set, descriptor] : m_DescriptorSet)
        {
            auto* write = descriptor.GetWriteDescriptor(MTL_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE, name);
            if (!write) continue;

            Renderer::Submit([write, index, accelerationStructure]() {
                write->BoundValue[index] = accelerationStructure;
            });
            return;
        }
    }
    void MetalShader::Set(const std::string &name, uint32_t index, Ref<BufferBase> buffer)
    {
        HZR_ASSERT(false, "TODO");
    }
    void MetalShader::Set(const std::string &name, Buffer buffer)
    {
        for (auto& [set, descriptor] : m_DescriptorSet)
        {
            auto* write = descriptor.GetWriteDescriptor(MTL_DESCRIPTOR_TYPE_PUSH_CONSTANT, name);
            if (!write) continue;

            Buffer data = Buffer::Copy(buffer);
            
            Renderer::Submit([write, data]() mutable {
                write->Buffer.Write(data.Data, data.Size);
                data.Release();
            });
            return;
        }
    }

    void MetalShader::Set(uint32_t index, Ref<VertexBuffer> buffer, size_t offset)
    {
        Ref<MetalShader> instance = this;
        Ref<MetalVertexBuffer> buff = buffer.As<MetalVertexBuffer>();
        
        Renderer::Submit([instance, buff, index, offset]() mutable {
            instance->m_InputBuffers[index] = { buff, offset };
        });
    }
    
    void MetalShader::BindResources(MTL::RenderCommandEncoder* encoder)
    {
        for(auto& [set, descriptor] : m_DescriptorSet)
            descriptor.BindGraphicsResources(encoder);
        
        for(auto& [index, buffer] : m_InputBuffers)
        {
            encoder->setVertexBuffer(buffer.Buffer->GetMetalBuffer(), 0, index + 28);
            encoder->setVertexBufferOffset(buffer.Offset, index + 28);
        }
    }
    void MetalShader::BindResources(MTL::ComputeCommandEncoder* encoder)
    {
        for(auto& [set, descriptor] : m_DescriptorSet)
            descriptor.BindComputeResources(encoder);
    }
    void MetalShader::Reflect()
    {
        m_ShaderData = ShaderCompiler::GetShaderResources(m_ShaderCode);

        for (auto& [set, buffers] : m_ShaderData.UniformsDescriptions)
        {
            MetalDescriptorSet& descriptorSet = m_DescriptorSet[set];
            for (auto& [binding, buffer] : buffers)
            {
                UniformBufferCreateInfo bufferInfo = {};
                bufferInfo.Name = buffer.Name;
                bufferInfo.Set = set;
                bufferInfo.Binding = binding;
                bufferInfo.Size = buffer.Size;
                bufferInfo.Usage = buffer.UsageFlags;
                
                MetalWriteDescriptor writeDescriptor = {};
                writeDescriptor.Type = MTL_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                writeDescriptor.DebugName = buffer.Name;
                writeDescriptor.Binding = binding;
                writeDescriptor.ArraySize = 0;
                writeDescriptor.BoundValue[0] = UniformBuffer::Create(&bufferInfo);
                writeDescriptor.Flags = buffer.UsageFlags;

                descriptorSet.AddWriteDescriptor(writeDescriptor);
            }
        }
        
        for (auto& [set, constants] : m_ShaderData.PushConstants)
        {
            MetalDescriptorSet& descriptorSet = m_DescriptorSet[set];
            for (auto& [binding, constant] : constants)
            {
                MetalWriteDescriptor writeDescriptor = {};
                writeDescriptor.Type = MTL_DESCRIPTOR_TYPE_PUSH_CONSTANT;
                writeDescriptor.DebugName = constant.Name;
                writeDescriptor.Binding = binding;
                writeDescriptor.ArraySize = 0;
                writeDescriptor.Size = constant.Size;
                writeDescriptor.Buffer.Allocate(writeDescriptor.Size);
                writeDescriptor.Buffer.ZeroInitialize();
                writeDescriptor.Flags = constant.UsageFlags;

                descriptorSet.AddWriteDescriptor(writeDescriptor);
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
