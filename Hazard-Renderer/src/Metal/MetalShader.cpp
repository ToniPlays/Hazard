#include "MetalShader.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalShaderCompiler.h"
#include "MetalContext.h"
#include "Core/Renderer.h"
#include "MetalGPUBuffer.h"

#include "Core/Window.h"

namespace HazardRenderer::Metal
{
    MetalShader::MetalShader(const std::unordered_map<uint32_t, std::string>& shaderModules)
    {
        for(auto& [stage, code] : shaderModules)
            m_ShaderCode[stage] = Buffer::Copy(code.data(), code.length());
        
        Reload();
    }
    MetalShader::~MetalShader()
    {
        
    }
    void MetalShader::Reload()
    {
        HZR_PROFILE_FUNCTION();
        
        auto device = MetalContext::GetMetalDevice();
        
        for(auto& [stage, code] : m_ShaderCode)
        {
            std::string msl((char*)code.Data, code.Size);
            
            NS::Error* libError;
            NS::String* source = NS::String::alloc()->string(msl.c_str(), NS::UTF8StringEncoding);
            MTL::CompileOptions* options = MTL::CompileOptions::alloc()->init();
            options->setFastMathEnabled(true);
            options->setPreserveInvariance(true);
            options->setLanguageVersion(MTL::LanguageVersion2_4);
            
            MTL::Library* lib = device->GetMetalDevice()->newLibrary(source, options, &libError);
            
            source->release();
            
            if(libError->code() != 0)
            {
                RenderMessage message = {};
                message.Severity = Severity::Error;
                message.Description = libError->description()->utf8String();
                message.StackTrace = msl;
                
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
    }
}
#endif