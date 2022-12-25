#include "MetalShaderCompiler.h"

#include "Profiling/Timer.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_msl.hpp>

namespace HazardRenderer::Metal
{
    static shaderc_shader_kind ShaderStageToShaderC(const ShaderStage& type)
    {
        switch (type)
        {
        case ShaderStage::Vertex:        return shaderc_glsl_vertex_shader;
        case ShaderStage::Fragment:        return shaderc_glsl_fragment_shader;
        case ShaderStage::Compute:        return shaderc_glsl_compute_shader;
        case ShaderStage::Geometry:        return shaderc_glsl_geometry_shader;
        case ShaderStage::Raygen:        return shaderc_raygen_shader;
        case ShaderStage::Miss:            return shaderc_miss_shader;
        case ShaderStage::ClosestHit:   return shaderc_closesthit_shader;
        case ShaderStage::AnyHit:       return shaderc_anyhit_shader;
        case ShaderStage::None:         return (shaderc_shader_kind)0;
        }
        return (shaderc_shader_kind)0;
    }


    MetalShaderCompiler::~MetalShaderCompiler()
    {
        m_ResultBinary.Release();
    }
    bool MetalShaderCompiler::Compile(CompileInfo* compileInfo)
    {
        HZR_PROFILE_FUNCTION();
        Timer timer;

        m_ResultBinary.Release();

        shaderc::CompileOptions options;

        switch (compileInfo->Renderer)
        {
        case RenderAPI::Vulkan: options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2); break;
        default: break;
        }
        
        options.SetTargetSpirv(shaderc_spirv_version_1_4);
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
        return succeeded;
    }
    bool MetalShaderCompiler::Decompile(Buffer binary, std::string &result)
    {
        HZR_PROFILE_FUNCTION();
        m_ErrorMessage.clear();
        spirv_cross::CompilerMSL compiler((uint32_t*)binary.Data, binary.Size / sizeof(uint32_t));
        result = compiler.compile();
        return !result.empty();
    }
}
