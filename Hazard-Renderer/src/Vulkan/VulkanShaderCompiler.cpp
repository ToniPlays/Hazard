#include "VulkanShaderCompiler.h"

#ifdef HZR_DESKTOP

#include "MathCore.h"
#include "Profiling/Timer.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace HazardRenderer::Vulkan 
{
    static shaderc_shader_kind ShaderStageToShaderC(uint32_t type) 
    {
        switch (type)
        {
        case SHADER_STAGE_VERTEX_BIT:	    return shaderc_glsl_vertex_shader;
        case SHADER_STAGE_FRAGMENT_BIT:	    return shaderc_glsl_fragment_shader;
        case SHADER_STAGE_COMPUTE_BIT:	    return shaderc_glsl_compute_shader;
        case SHADER_STAGE_GEOMETRY_BIT:	    return shaderc_glsl_geometry_shader;
        case SHADER_STAGE_RAYGEN_BIT:	    return shaderc_raygen_shader;
        case SHADER_STAGE_MISS_BIT:	        return shaderc_miss_shader;
        case SHADER_STAGE_CLOSEST_HIT_BIT:  return shaderc_closesthit_shader;
        case SHADER_STAGE_ANY_HIT_BIT:      return shaderc_anyhit_shader;
        case SHADER_STAGE_NONE:             return (shaderc_shader_kind)0;
        }
        return (shaderc_shader_kind)0;
    }

    VulkanShaderCompiler::~VulkanShaderCompiler()
    {
        HZR_PROFILE_FUNCTION();
        m_ResultBinary.Release();
    }

    bool VulkanShaderCompiler::Compile(CompileInfo* compileInfo)
    {
        HZR_PROFILE_FUNCTION();
        Timer timer;

        m_ResultBinary.Release();

        shaderc::CompileOptions options;

        switch (compileInfo->Renderer)
        {
        case RenderAPI::Vulkan: options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2); break;
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
            ShaderStageToShaderC((uint32_t)compileInfo->Stage), compileInfo->Name.c_str(), options);

        bool succeeded = result.GetCompilationStatus() == shaderc_compilation_status_success;
        if (succeeded)
            m_ResultBinary = Buffer::Copy(result.begin(), (result.end() - result.begin()) * sizeof(uint32_t));
        else
            m_ErrorMessage = result.GetErrorMessage();

        m_CompilationTime = timer.ElapsedMillis();
        return succeeded;
    }
}
#endif
