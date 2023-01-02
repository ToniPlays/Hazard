#include "MetalShaderCompiler.h"

#include "Profiling/Timer.h"

#ifdef HZR_PLATFORM_MACOS
    #include <shaderc/shaderc.hpp>
    #include <spirv_cross/spirv_msl.hpp>
#endif

namespace HazardRenderer::Metal
{
#ifdef HZR_PLATFORM_MACOS
    static shaderc_shader_kind ShaderStageToShaderC(const ShaderStage& type)
    {
        switch (type)
        {
        case ShaderStage::Vertex:       return shaderc_vertex_shader;
        case ShaderStage::Fragment:     return shaderc_fragment_shader;
        case ShaderStage::Compute:      return shaderc_compute_shader;
        case ShaderStage::Geometry:     return shaderc_geometry_shader;
        case ShaderStage::Raygen:       return shaderc_raygen_shader;
        case ShaderStage::Miss:         return shaderc_miss_shader;
        case ShaderStage::ClosestHit:   return shaderc_closesthit_shader;
        case ShaderStage::AnyHit:       return shaderc_anyhit_shader;
        case ShaderStage::None:         return (shaderc_shader_kind)0;
        }
        return (shaderc_shader_kind)0;
    }
#endif

    MetalShaderCompiler::~MetalShaderCompiler()
    {
        m_ResultBinary.Release();
    }
    bool MetalShaderCompiler::Compile(CompileInfo* compileInfo)
    {

        HZR_PROFILE_FUNCTION();
        Timer timer;

        m_ResultBinary.Release();
#ifdef HZR_PLATFORM_MACOS
        shaderc::CompileOptions options;

        switch (compileInfo->Renderer)
        {
        case RenderAPI::Vulkan: options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3); break;
        default: break;
        }
        
        options.SetTargetSpirv(shaderc_spirv_version_1_4);
        options.SetSourceLanguage(shaderc_source_language_glsl);
        options.SetOptimizationLevel(shaderc_optimization_level_zero);
        options.SetOptimizationLevel((shaderc_optimization_level)compileInfo->Optimization);
        options.SetGenerateDebugInfo();

        for (uint32_t i = 0; i < compileInfo->DefineCount; i++)
        {
            ShaderDefine define = compileInfo->pDefines[i];
            if (!define.Value.empty())
                options.AddMacroDefinition(define.Name, define.Value);
            else options.AddMacroDefinition(define.Name);
        }

        shaderc::Compiler compiler;
        
        shaderc::CompilationResult result = compiler.CompileGlslToSpv(compileInfo->Source,
            ShaderStageToShaderC(compileInfo->Stage), compileInfo->Name.c_str(), options);

        bool succeeded = result.GetCompilationStatus() == shaderc_compilation_status_success;
        if (succeeded)
            m_ResultBinary = Buffer::Copy(result.begin(), (result.end() - result.begin()) * sizeof(uint32_t));
        else
            m_ErrorMessage = result.GetErrorMessage();
        m_CompilationTime = timer.ElapsedMillis();
#else
        bool succeeded = false;
#endif
        return succeeded;
    }
    bool MetalShaderCompiler::Decompile(Buffer binary, std::string &result, bool tesellation)
    {
        HZR_PROFILE_FUNCTION();
        
        m_ErrorMessage.clear();
#ifdef HZR_PLATFORM_MACOS
        
        spirv_cross::CompilerMSL::Options options;
        options.set_msl_version(2, 4);
        options.vertex_for_tessellation = tesellation;
        options.texture_buffer_native = true;
        options.
        
        spirv_cross::CompilerMSL compiler((uint32_t*)binary.Data, binary.Size / sizeof(uint32_t));
        
        compiler.set_msl_options(options);
        result = compiler.compile();
#endif
    
        return !result.empty();
    }
    std::unordered_map<std::string, MSLBinding> MetalShaderCompiler::GetMSLBindings(Buffer binary, bool tesellation)
    {
        std::unordered_map<std::string, MSLBinding> result;
        
#ifdef HZR_PLATFORM_MACOS
        spirv_cross::CompilerMSL::Options options;
        options.set_msl_version(2, 4);
        options.vertex_for_tessellation = tesellation;
        options.texture_buffer_native = true;
        
        spirv_cross::CompilerMSL compiler((uint32_t*)binary.Data, binary.Size / sizeof(uint32_t));
        
        compiler.set_msl_options(options);
        auto r = compiler.compile();
        
        auto resources = compiler.get_shader_resources();
        
        for(auto& buffer : resources.uniform_buffers)
        {
            int id = compiler.get_automatic_msl_resource_binding(buffer.id);
            if(id == -1) continue;
            result[buffer.name].Binding = id;
        }
        for(auto& range : resources.push_constant_buffers)
        {
            int id = compiler.get_automatic_msl_resource_binding(range.id);
            if(id == -1) continue;
            result[range.name].Binding = id;
        }
        
        for(auto& image : resources.storage_images)
        {
            int id = compiler.get_automatic_msl_resource_binding(image.id);
            auto& type = compiler.get_type(image.type_id);
            uint32_t sId = compiler.get_automatic_msl_resource_binding_secondary(image.id);
            
            if(id == -1) continue;
            
            result[image.name].Binding = id;
            result[image.name].SamplerBinding = sId < UINT32_MAX ? sId : id;
        }
        
        for(auto& sampler : resources.sampled_images)
        {
            int id = compiler.get_automatic_msl_resource_binding(sampler.id);
            auto& type = compiler.get_type(sampler.type_id);
            uint32_t sId = compiler.get_automatic_msl_resource_binding_secondary(sampler.id);
            
            if(id == -1) continue;
            
            result[sampler.name].Binding = id;
            result[sampler.name].SamplerBinding = sId < UINT32_MAX ? sId : id;
        }
#endif
        return result;
    }
}
