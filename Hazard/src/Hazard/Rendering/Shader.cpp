#pragma once

#include <hzrpch.h>
#include "Shader.h"
#include "RenderCommand.h"

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
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HZR_CORE_ASSERT(eol != std::string::npos, "Syntax Error, check Shader");
			size_t begin = pos + typeTokenLen + 1;
			std::string type = source.substr(begin, eol - begin);

			HZR_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type {0}", type);
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			HZR_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax Error, check Shader");

			pos = source.find(typeToken, nextLinePos);

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;

	}
	ShaderStageData Shader::ProcessShaderStage(const spirv_cross::Compiler& compiler, const spirv_cross::ShaderResources& resources)
	{
		HZR_PROFILE_FUNCTION();
		ShaderStageData shaderStage;

		uint32_t stride = 0;
		for (auto& resource : resources.stage_inputs) {

			auto spvType = compiler.get_type(resource.base_type_id);
			ShaderStageInput input;
			input.Name = resource.name;
			input.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
			input.Location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);
			input.Type = Rendering::Utils::ShaderTypeFromSPV(spvType);
			input.Size = ShaderDataTypeSize(input.Type);

			stride += input.Size;
			shaderStage.Inputs[input.Location] = input;
		}
		shaderStage.Stride = stride;
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

		for (auto& resource : resources.uniform_buffers) {
			ShaderUniform uniform;
			auto& type = compiler.get_type(resource.base_type_id);

			uniform.Name = resource.name;
			uniform.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
			uniform.MemberCount = type.member_types.size();
			uniform.Size = compiler.get_declared_struct_size(type);
			
			shaderStage.Uniforms.push_back(uniform);
		}
		return shaderStage;
	}
}
