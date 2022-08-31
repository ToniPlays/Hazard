#include "OpenGLShaderCompiler.h"
#ifdef HZR_INCLUDE_OPENGL

#include "MathCore.h"
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace HazardRenderer::OpenGL
{
	static shaderc_shader_kind ShaderStageToShaderC(const ShaderStage& type) {
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

	bool OpenGLShaderCompiler::Compile(CompileInfo* compileInfo)
	{
		Timer timer;

		m_ResultBinary.clear();

		shaderc::CompileOptions options;

		switch (compileInfo->Renderer)
		{
		case RenderAPI::Vulkan: options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2); break;
		case RenderAPI::OpenGL: options.SetTargetEnvironment(shaderc_target_env_opengl, 450); break;
		}

		options.SetOptimizationLevel((shaderc_optimization_level)compileInfo->Optimization);
		options.SetGenerateDebugInfo();

		for (auto& [name, value] : compileInfo->Defines) {
			if (!value.empty())
				options.AddMacroDefinition(name, value);
			else options.AddMacroDefinition(name);
		}
		shaderc::Compiler compiler;
		shaderc::CompilationResult result = compiler.CompileGlslToSpv(compileInfo->Source,
			ShaderStageToShaderC(compileInfo->Stage), compileInfo->Name.c_str(), options);

		bool succeeded = result.GetCompilationStatus() == shaderc_compilation_status_success;
		if (succeeded)
			m_ResultBinary = { result.begin(), result.end() };
		else
			m_ErrorMessage = result.GetErrorMessage();

		m_CompilationTime = timer.ElapsedMillis();
		return succeeded;
	}
	bool OpenGLShaderCompiler::Decompile(const std::vector<uint32_t> binary, std::string& result)
	{
		spirv_cross::CompilerGLSL compiler(binary);
		result = compiler.compile();
		return !result.empty();
	}
	ShaderData OpenGLShaderCompiler::GetShaderResources(const std::unordered_map<ShaderStage, std::vector<uint32_t>> binaries)
	{
		ShaderData result;

		for (auto& [stage, binary] : binaries)
		{
			spirv_cross::Compiler compiler(binary);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			//Check stage inputs
			ShaderStageData& data = result.Stages[stage];
			for (auto& resource : resources.stage_inputs)
			{
				auto spvType = compiler.get_type(resource.base_type_id);
				uint32_t location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);

				ShaderStageInput& input = data.Inputs[location];
				input.Name = resource.name;
				input.Location = location;
				input.Type = Utils::ShaderDataTypeFromSPV(spvType);
				input.Size = ShaderDataTypeSize(input.Type);
				input.Offset = compiler.get_decoration(resource.id, spv::Decoration::DecorationOffset);
			}

			for (auto& resource : resources.stage_outputs)
			{
				auto& spvType = compiler.get_type(resource.base_type_id);
				uint32_t location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);

				ShaderStageOutput& output = data.Outputs[location];
				output.Name = resource.name;
				output.Location = location;
				output.Type = Utils::ShaderDataTypeFromSPV(spvType);
				output.Size = ShaderDataTypeSize(output.Type);
			}

			//Sort input offsets
			uint32_t offset = 0;
			for (uint32_t i = 0; i < data.Inputs.size(); i++)
			{
				auto& input = data.Inputs[i];
				input.Offset = offset;
				offset += input.Size;
			}

			for (auto& resource : resources.uniform_buffers)
			{
				auto& spvType = compiler.get_type(resource.base_type_id);
				uint32_t set = compiler.get_decoration(resource.id, spv::Decoration::DecorationDescriptorSet);
				auto& descriptorSet = result.UniformsDescriptions[set];

				uint32_t binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);

				if (descriptorSet.find(binding) != descriptorSet.end())
				{
					auto& buffer = descriptorSet[binding];
					buffer.UsageFlags |= (uint32_t)stage;
				}
				else
				{
					auto& buffer = descriptorSet[binding];
					buffer.Name = resource.name;
					buffer.Binding = binding;
					buffer.DescritorSet = set;
					buffer.MemberCount = spvType.member_types.size();
					buffer.Size = compiler.get_declared_struct_size(spvType);
					buffer.UsageFlags |= (uint32_t)stage;
				}
			}
			for (auto& resource : resources.sampled_images)
			{
				auto& spvType = compiler.get_type(resource.base_type_id);
				auto& type = compiler.get_type(resource.type_id);
				uint32_t set = compiler.get_decoration(resource.id, spv::Decoration::DecorationDescriptorSet);
				uint32_t binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);

				auto& descriptorSet = result.ImageSamplers[set];

				auto& sampler = descriptorSet[binding];
				sampler.Name = resource.name;
				sampler.Binding = binding;
				sampler.DescritorSet = set;
				sampler.ArraySize = Math::Max<uint32_t>(type.array[0], 1);
				sampler.Dimension = spvType.image.dim;
			}
		}
		return result;
	}
	void OpenGLShaderCompiler::PrintReflectionData(const ShaderData& data)
	{
		std::cout << "Reflection data" << std::endl;
		for (auto& [stage, data] : data.Stages)
		{
			std::cout << "- Shader stage: " << Utils::ShaderStageToString((uint32_t)stage) << std::endl;
			std::cout << "  - Inputs:" << std::endl;
			for (auto& [location, input] : data.Inputs)
			{
				std::cout << "    - Location: " << location << std::endl;
				std::cout << "      - Name: " << input.Name << std::endl;
				std::cout << "      - Type: " << ShaderDataTypeToString(input.Type) << std::endl;
				std::cout << "      - Size: " << input.Size << std::endl;
				std::cout << "      - Offset: " << input.Offset << std::endl;
			}
			std::cout << "  - Outputs:" << std::endl;
			for (auto& [location, ouput] : data.Outputs)
			{
				std::cout << "    - Location: " << location << std::endl;
				std::cout << "      - Name: " << ouput.Name << std::endl;
				std::cout << "      - Type: " << ShaderDataTypeToString(ouput.Type) << std::endl;
				std::cout << "      - Size: " << ouput.Size << std::endl;
			}
		}

		std::cout << "  - UniformBuffers:" << std::endl;
		for (auto& [set, bindings] : data.UniformsDescriptions)
		{
			std::cout << "    - Set: " << set << std::endl;
			for (auto& [binding, buffer] : bindings) {
				std::cout << "      - Binding: " << binding << std::endl;
				std::cout << "        - Name: " << buffer.Name << std::endl;
				std::cout << "        - Size: " << buffer.Size << std::endl;
				std::cout << "        - Usage: " << Utils::UsageFlagsToString(buffer.UsageFlags) << std::endl;
			}
		}
		std::cout << "  - Image samplers:" << std::endl;
		for (auto& [set, samplers] : data.ImageSamplers)
		{
			std::cout << "    - Set: " << set << std::endl;
			for (auto& [binding, sampler] : samplers) {
				std::cout << "      - Binding: " << binding << std::endl;
				std::cout << "        - Name: " << sampler.Name << std::endl;
				std::cout << "        - Dimension: " << sampler.Dimension << std::endl;
				std::cout << "        - ArraySize: " << sampler.ArraySize << std::endl;
			}
		}

		std::cout << "\n\n" << std::endl;
	}
}
#endif