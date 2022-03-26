
#include "MetalShader.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalContext.h"
#include "Core/Pipeline/ShaderFactory.h"
#include <Metal/Metal.hpp>

#include "Utility/StringUtil.h"
#include "File/File.h"

using namespace HazardUtility;

namespace HazardRenderer::Metal
{
    MetalShader::MetalShader(const std::string& path)
    {
        MTL::Device* device = MetalContext::GetMetalDevice();
        auto sources = GetMetalShaderSources(path);
        
        
        for(auto& [type, source] : sources) {
            NS::Error* libError;
            NS::String* shaderSource = NS::String::string()->string(source.c_str(), NS::UTF8StringEncoding);
            MTL::CompileOptions* options = MTL::CompileOptions::alloc()->init();
            MTL::Library* lib = device->newLibrary(shaderSource, options, &libError);
            
            //if(libError)
            //    std::cout << libError->description()->utf8String() << std::endl;

            NS::Error* functionError;
            std::string shaderTypeName = Utils::ShaderTypeToString(type);
            NS::String* name = NS::String::string()->string(shaderTypeName.c_str(), NS::UTF8StringEncoding);
            MTL::FunctionDescriptor* descriptor = MTL::FunctionDescriptor::functionDescriptor();
            
            descriptor->setName(name);
            
            m_Functions[type] = lib->newFunction(descriptor, &functionError);
            
            //if(functionError)
            //    std::cout << functionError->description()->utf8String() << std::endl;
        }
    }
    MetalShader::MetalShader(const std::string& name, const std::string& vertexPath, const std::string& fragmenPath)
    {
        
    }
    MetalShader::~MetalShader() {
        
    }
    void MetalShader::Reload() {
        
    }
    void MetalShader::Bind(Ref<RenderCommandBuffer> cmdBufer) {
        
    }
    void MetalShader::Unbind() {
        
    }
    void MetalShader::SetUniformBuffer(const std::string &name, void *data, uint32_t size) {
        //m_UniformBuffers[name]->SetData(data);
    }
    void MetalShader::Set(const std::string& name, uint32_t index, uint32_t value) {
        
    }
    void MetalShader::Set(const std::string& name, uint32_t index, Ref<Texture2D>& value) {
        
    }
    std::unordered_map<ShaderType, std::string> MetalShader::GetMetalShaderSources(const std::string& path) {
            
        std::filesystem::path abs = File::GetFileAbsolutePath(path);
        std::string contents = File::ReadFile(abs);
        std::unordered_map<ShaderType, std::string> shaderSources;

        const char* typeToken = "#type";
        StringUtil::PreprocessTypeSource(contents, typeToken, [&](std::string type, std::string source) {
            shaderSources[Utils::ShaderTypeFromString(type)] = source;
            });

        return shaderSources;
    }
}
#endif
