#pragma once

#include <hzrpch.h>
#include "VulkanShader.h"
#include "VulkanContext.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/File/File.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Hazard::Rendering::Vulkan
{
	namespace Utils
	{
		static std::string ShaderStageCachedVulkanExtension(ShaderType type) {
			switch (type)
			{
			case Vertex:	return ".cached_vulkan.vert";
			case Fragment:	return ".cached_vulkan.frag";
			}
			HZR_CORE_ASSERT(false);
			return "";
		}
		static VkShaderStageFlagBits ShaderTypeToVkType(ShaderType type) {
			switch (type)
			{
			case Vertex:	return VK_SHADER_STAGE_VERTEX_BIT;
			case Fragment:	return VK_SHADER_STAGE_FRAGMENT_BIT;
			case Compute:	return VK_SHADER_STAGE_COMPUTE_BIT;
			case Geometry:	return VK_SHADER_STAGE_GEOMETRY_BIT;
			}
			HZR_CORE_ASSERT(false);
			return (VkShaderStageFlagBits)0;
		}
		static ShaderType ShaderTypeFromVkType(VkShaderStageFlagBits type) {
			switch (type)
			{
			case VK_SHADER_STAGE_VERTEX_BIT:	return Vertex;
			case VK_SHADER_STAGE_FRAGMENT_BIT:	return Fragment;
			case VK_SHADER_STAGE_COMPUTE_BIT:	return Compute;
			case VK_SHADER_STAGE_GEOMETRY_BIT:	return Geometry;
			}
			HZR_CORE_ASSERT(false);
			return ShaderType::Unknown;
		}
		static std::string VkTypeToString(VkShaderStageFlagBits type) {
			switch (type)
			{
			case VK_SHADER_STAGE_VERTEX_BIT:	return "Vertex";
			case VK_SHADER_STAGE_FRAGMENT_BIT:	return "Fragment";
			case VK_SHADER_STAGE_COMPUTE_BIT:	return "Compute";
			case VK_SHADER_STAGE_GEOMETRY_BIT:	return "Geometry";
			}
			HZR_CORE_ASSERT(false);
			return "";
		}
		static shaderc_shader_kind VkShaderStageToShaderC(VkShaderStageFlags stage) {
			switch (stage)
			{
			case VK_SHADER_STAGE_VERTEX_BIT:		return shaderc_glsl_vertex_shader;
			case VK_SHADER_STAGE_FRAGMENT_BIT:		return shaderc_glsl_fragment_shader;
			case VK_SHADER_STAGE_COMPUTE_BIT:		return shaderc_glsl_compute_shader;
			case VK_SHADER_STAGE_GEOMETRY_BIT:		return shaderc_glsl_geometry_shader;
			}
			HZR_ASSERT(false);
			return (shaderc_shader_kind)0;
		}
	}

	VulkanShader::VulkanShader(const std::string& file) : m_Path(file)
	{
		Reload();
	}
	VulkanShader::~VulkanShader()
	{

	}
	void VulkanShader::Reload()
	{
		Timer timer;
		std::cout << "Reloading Shader " << m_Path << std::endl;

		std::string result = File::ReadFile(m_Path);
		auto shaderSources = PreProcess(result);

		CompileOrGetVulkanBinaries(shaderSources);
		Reflect();
		CreateModules();

		HZR_CORE_TRACE("[VulkanShader]: Creation took {0} ms", timer.ElapsedMillis());

	}
	void VulkanShader::Bind()
	{

	}
	void VulkanShader::Unbind()
	{

	}
	void VulkanShader::GetStageInfo(std::vector<VkPipelineShaderStageCreateInfo>& info)
	{
		info.resize(m_ShaderCode.size());

		uint32_t i = 0;
		for (auto&& [stage, source] : m_ShaderCode) {
			info[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			info[i].stage = stage;
			info[i].module = m_Modules[stage];
			info[i].pName = "main";
			i++;
		}

	}
	void VulkanShader::DestroyModules()
	{
		auto device = VulkanContext::GetDevice()->GetDevice();

		for (auto&& [stage, module] : m_Modules) {
			vkDestroyShaderModule(device, module, nullptr);
		}
	}
	void VulkanShader::CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources)
	{
		std::filesystem::path cache = RenderEngine::GetShaderCompilePath();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		options.SetOptimizationLevel(shaderc_optimization_level_performance);

		m_ShaderCode.clear();

		for (auto&& [stage, source] : sources)
		{
			std::filesystem::path shaderFilePath = m_Path;
			std::filesystem::path cachedFilePath = cache / (shaderFilePath.filename().string() + Utils::ShaderStageCachedVulkanExtension(stage));

			VkShaderStageFlagBits vkStage = Utils::ShaderTypeToVkType(stage);

			if (File::Exists(cachedFilePath)) {
				if (!File::IsNewerThan(shaderFilePath, cachedFilePath)) {
					File::ReadBinaryFileUint32(cachedFilePath, m_ShaderCode[vkStage]);
					continue;
				}
			}

			HZR_CORE_INFO("[VulkanShader]: Reload, source modified");
			shaderc::CompilationResult module = compiler.CompileGlslToSpv(source, Utils::VkShaderStageToShaderC(Utils::ShaderTypeToVkType(stage)), m_Path.c_str(), options);

			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				HZR_CORE_ERROR("Error {0} : {1}, using cached shader binary", Rendering::Utils::ShaderTypeToString(stage), module.GetErrorMessage());
				File::ReadBinaryFileUint32(cachedFilePath, m_ShaderCode[vkStage]);
				continue;
			}

			m_ShaderCode[vkStage] = std::vector<uint32_t>(module.begin(), module.cend());
			File::WriteBinaryFile(cachedFilePath, m_ShaderCode[vkStage]);
		}
	}
	void VulkanShader::CreateModules()
	{
		auto device = VulkanContext::GetDevice()->GetDevice();

		for (auto&& [stage, binary] : m_ShaderCode) {
			VkShaderModuleCreateInfo moduleInfo = {};
			moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleInfo.codeSize = binary.size();
			moduleInfo.pCode = binary.data();

			vkCreateShaderModule(device, &moduleInfo, nullptr, &m_Modules[stage]);
		}
	}
	void VulkanShader::Reflect()
	{
		m_ShaderStageData.clear();

		for (auto&& [stage, binary] : m_ShaderCode) {
			ShaderStageData shaderStage;

			spirv_cross::Compiler compiler(binary);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			for (auto& resource : resources.stage_inputs) {

				auto spvType = compiler.get_type(resource.base_type_id);
				ShaderStageInput input;
				input.Name = resource.name;
				input.Location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);
				input.Type = Rendering::Utils::ShaderTypeFromSPV(spvType);
				input.Size = ShaderDataTypeSize(input.Type);

				shaderStage.Inputs.push_back(input);
			}
			for (auto& resource : resources.stage_outputs) {

				auto spvType = compiler.get_type(resource.base_type_id);
				ShaderStageOutput output;
				output.Name = resource.name;
				output.Location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);
				output.Type = Rendering::Utils::ShaderTypeFromSPV(spvType);
				output.Size = ShaderDataTypeSize(output.Type);

				shaderStage.Outputs.push_back(output);
			}

			m_ShaderStageData[Utils::ShaderTypeFromVkType(stage)] = shaderStage;
		}
	}
}
