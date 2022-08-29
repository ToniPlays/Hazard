#include "VulkanShaderCompiler.h"
#ifdef HZR_INCLUDE_VULKAN

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace HazardRenderer::Vulkan 
{
    static shaderc_shader_kind ShaderStageToShaderC(const ShaderStage& type) 
    {
        switch (type)
        {
        case ShaderStage::Vertex:	return shaderc_glsl_vertex_shader;
        case ShaderStage::Fragment:	return shaderc_glsl_fragment_shader;
        case ShaderStage::Compute:	return shaderc_glsl_compute_shader;
        case ShaderStage::Geometry:	return shaderc_glsl_geometry_shader;
        case ShaderStage::None:      return (shaderc_shader_kind)0;
        }
        return (shaderc_shader_kind)0;
    }

    bool VulkanShaderCompiler::Compile(CompileInfo* compileInfo)
    {
        Timer timer;
        std::cout << "Compiling: " << Utils::ShaderStageToString(compileInfo->Stage) << std::endl;

        m_ResultBinary.clear();

        shaderc::CompileOptions options;

        switch (compileInfo->Renderer)
        {
        case RenderAPI::Vulkan: options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2); break;
        }
        
        options.SetOptimizationLevel((shaderc_optimization_level)compileInfo->Optimization);
        options.SetGenerateDebugInfo();

        for (auto& [Name, value] : compileInfo->Defines) 
        {
            if (!value.empty())
                options.AddMacroDefinition(Name, value);
            else options.AddMacroDefinition(Name);
        }
        shaderc::Compiler compiler;
        shaderc::CompilationResult result = compiler.CompileGlslToSpv(compileInfo->Source, 
            ShaderStageToShaderC(compileInfo->Stage), compileInfo->Name.c_str(), options);

        bool succeeded = result.GetCompilationStatus() == shaderc_compilation_status_success;
        if (succeeded)
            m_ResultBinary = { result.begin(), result.end()};
        else
            m_ErrorMessage = result.GetErrorMessage();

        m_CompilationTime = timer.ElapsedMillis();
        return succeeded;
    }
    ShaderStageData VulkanShaderCompiler::GetResources(std::vector<uint32_t> binary)
    {
        spirv_cross::Compiler compiler(binary);
        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        ShaderStageData shaderStage;

        for (auto& resource : resources.stage_inputs) {

            auto spvType = compiler.get_type(resource.base_type_id);
            ShaderStageInput input;
            input.Name = resource.name;
            input.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
            input.Location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);
            input.Type = Utils::ShaderStageFromSPV(spvType);
            input.Size = ShaderDataTypeSize(input.Type);

            shaderStage.Inputs[input.Location] = input;
            shaderStage.Stride += input.Size;
        }
        uint32_t offset = 0;

        for (uint32_t i = 0; i < shaderStage.Inputs.size(); i++)
        {
            shaderStage.Inputs[i].Offset = offset;
            offset += shaderStage.Inputs[i].Size;
        }

        for (auto& resource : resources.stage_outputs) {

            auto spvType = compiler.get_type(resource.base_type_id);
            ShaderStageOutput output;
            output.Name = resource.name;
            output.Location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);
            output.Type = Utils::ShaderStageFromSPV(spvType);
            output.Size = ShaderDataTypeSize(output.Type);

            shaderStage.Outputs[output.Location] = output;
        }
        for (auto& resource : resources.sampled_images) {

            auto spvType = compiler.get_type(resource.base_type_id);
            auto& type = compiler.get_type(resource.type_id);
            uint32_t binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
            uint32_t arraySize = type.array[0] == 0 ? 1 : type.array[0];

            ShaderSampledImage output;
            output.Name = resource.name;
            output.Binding = binding;
            output.Dimension = spvType.image.dim;
            output.ArraySize = arraySize;

            shaderStage.SampledImages[binding] = output;
        }
        return shaderStage;
    }
}
#endif