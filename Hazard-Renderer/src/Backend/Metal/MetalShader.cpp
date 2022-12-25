
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
                break;
            }
            
            std::cout << result << std::endl;
            
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
        
        Ref<MetalShader> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            
            });
    }

    void MetalShader::Set(const std::string &name, uint32_t index, Ref<Image2D> image)
    {
        
    }
    void MetalShader::Set(const std::string &name, uint32_t index, Ref<CubemapTexture> cubemap)
    {
        
    }
    void MetalShader::Set(const std::string &name, uint32_t index, Ref<AccelerationStructure> accelerationStructure)
    {
        
    }
    void MetalShader::Set(const std::string &name, uint32_t index, Ref<BufferBase> buffer)
    {
        
    }
}
#endif
