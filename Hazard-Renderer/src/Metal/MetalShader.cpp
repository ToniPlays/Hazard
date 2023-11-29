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
        Renderer::SubmitResourceFree([code = m_ShaderCode, functions = m_Functions]() mutable {
            for(auto& [stage, buffer] : code)
                buffer.Release();
            
            for(auto [stage, func] : functions)
                func->release();
        });
    }
    void MetalShader::Reload()
    {
        HZR_PROFILE_FUNCTION();
        
        auto device = MetalContext::GetMetalDevice();
        
        for(auto& [stage, code] : m_ShaderCode)
        {
            std::string msl((char*)code.Data, code.Size);
            
            NS::Error* libError = nullptr;
            NS::String* source = NS::String::alloc()->string(msl.c_str(), NS::UTF8StringEncoding);
            
            MTL::CompileOptions* options = MTL::CompileOptions::alloc()->init();
            options->setFastMathEnabled(true);
            options->setPreserveInvariance(true);
            options->setLanguageVersion(MTL::LanguageVersion2_4);
            
            MTL::Library* lib = device->GetMetalDevice()->newLibrary(source, options, &libError);
            
            source->release();
            options->release();
            
            if(libError)
            {
                RenderMessage message = {};
                message.Severity = Severity::Error;
                message.Description = libError->description()->utf8String();
                message.StackTrace = msl;
                
                Window::SendDebugMessage(message);
                continue;
            }
            
            NS::Error* functionError = nullptr;
            NS::String* name = NS::String::alloc()->init("main0", NS::UTF8StringEncoding);
            
            MTL::FunctionDescriptor* descriptor = MTL::FunctionDescriptor::functionDescriptor();
            descriptor->setName(name);
            
            auto func = lib->newFunction(descriptor, &functionError);
            
            if(functionError)
            {
                RenderMessage message = {};
                message.Severity = Severity::Error;
                message.Description = functionError->description()->utf8String();
                message.StackTrace = code;
                
                Window::SendDebugMessage(message);
                continue;
            }
            
            name->release();
            descriptor->release();
            lib->release();
            
            m_Functions[stage] = func;
        }
    }
}
#endif
