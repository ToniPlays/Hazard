#include "MetalShader.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalShaderCompiler.h"
#include "MetalContext.h"
#include "Backend/Core/Renderer.h"
#include "MetalGPUBuffer.h"

#include "Backend/Core/Window.h"

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
            
            if(!compiler.Decompile(spirv, result, stage == SHADER_STAGE_RAYGEN_BIT))
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
    }
}
#endif
