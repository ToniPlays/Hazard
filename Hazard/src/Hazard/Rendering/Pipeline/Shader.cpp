#pragma once

#include <hzrpch.h>
#include "Shader.h"
#include "../RenderCommand.h"
#include "Hazard/Utils/StringUtil.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering 
{

    Ref<Shader> Shader::Create(const std::string& path)
    {
		HZR_PROFILE_FUNCTION();
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLShader>::Create(path);
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanShader>::Create(path);
		}
    }
	std::unordered_map<ShaderType, std::string> Shader::PreProcess(const std::string& source)
	{
		HZR_PROFILE_FUNCTION();
		std::unordered_map<ShaderType, std::string> shaderSources;

		const char* typeToken = "#type";
		StringUtil::PreprocessTypeSource(source, typeToken, [&](std::string type, std::string source) {
			shaderSources[Utils::ShaderTypeFromString(type)] = source;
		});

		return shaderSources;

	}
	ShaderStageData Shader::ProcessShaderStage(const spirv_cross::Compiler& compiler, const spirv_cross::ShaderResources& resources)
	{
		HZR_PROFILE_FUNCTION();
		ShaderStageData shaderStage;

		for (auto& resource : resources.stage_inputs) {

			auto spvType = compiler.get_type(resource.base_type_id);
			ShaderStageInput input;
			input.Name = resource.name;
			input.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
			input.Location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);
			input.Type = Rendering::Utils::ShaderTypeFromSPV(spvType);
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
			output.Type = Rendering::Utils::ShaderTypeFromSPV(spvType);
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
