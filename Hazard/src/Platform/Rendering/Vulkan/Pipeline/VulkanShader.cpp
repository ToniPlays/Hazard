#pragma once

#include <hzrpch.h>
#include "VulkanShader.h"
#include "../VulkanContext.h"
#include "../VKUtils.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/File/File.h"
#include "VulkanBuffers.h"

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
			HZR_CORE_ASSERT(false, "[VulkanShader]: No cache file extension for {0} shader", Rendering::Utils::ShaderTypeToString(type));
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
			HZR_CORE_ASSERT(false, "[VulkanShader]: Undefined ShaderType {0}", Rendering::Utils::ShaderTypeToString(type));
			return (VkShaderStageFlagBits)0;
		}
		static std::string VkTypeToString(VkShaderStageFlagBits type) {
			switch (type)
			{
			case VK_SHADER_STAGE_VERTEX_BIT:	return "Vertex";
			case VK_SHADER_STAGE_FRAGMENT_BIT:	return "Fragment";
			case VK_SHADER_STAGE_COMPUTE_BIT:	return "Compute";
			case VK_SHADER_STAGE_GEOMETRY_BIT:	return "Geometry";
			}
			HZR_CORE_ASSERT(false, "[VulkanShader]: Unknown conversion from ShaderStageFlag to string");
			return "";
		}
		static ShaderType ShaderTypeFromVkType(VkShaderStageFlagBits type) {
			switch (type)
			{
			case VK_SHADER_STAGE_VERTEX_BIT:	return Vertex;
			case VK_SHADER_STAGE_FRAGMENT_BIT:	return Fragment;
			case VK_SHADER_STAGE_COMPUTE_BIT:	return Compute;
			case VK_SHADER_STAGE_GEOMETRY_BIT:	return Geometry;
			}
			HZR_CORE_ASSERT(false, "[VulkanShader]: Undefined Vulkan ShaderStageFlags {0}", VkTypeToString(type));
			return ShaderType::Unknown;
		}
		static shaderc_shader_kind VkShaderStageToShaderC(VkShaderStageFlagBits stage) {
			switch (stage)
			{
			case VK_SHADER_STAGE_VERTEX_BIT:		return shaderc_glsl_vertex_shader;
			case VK_SHADER_STAGE_FRAGMENT_BIT:		return shaderc_glsl_fragment_shader;
			case VK_SHADER_STAGE_COMPUTE_BIT:		return shaderc_glsl_compute_shader;
			case VK_SHADER_STAGE_GEOMETRY_BIT:		return shaderc_glsl_geometry_shader;
			}
			HZR_CORE_ASSERT(false, "[VulkanShader]: UNdefined conversion from {0} to ShaderC type", VkTypeToString(stage));
			return (shaderc_shader_kind)0;
		}
	}

	VulkanShader::VulkanShader(const std::string& file) : m_Path(file)
	{
		HZR_PROFILE_FUNCTION();
		Reload();
	}
	VulkanShader::~VulkanShader()
	{
		DestroyModules();
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
	void VulkanShader::SetUniformBuffer(const std::string& name, void* data)
	{
		auto& uniformBuffer = m_UniformBuffers[name];
		HZR_CORE_ASSERT(uniformBuffer, "[VulkanShader]: UniformBuffer '{0}' does not exist", name);
		uniformBuffer->SetData(data);
	}
	VkResult VulkanShader::CreateDescriptorLayout(VkDescriptorSetLayout* layout)
	{
		return VkResult();
	}
	VkVertexInputBindingDescription VulkanShader::GetBindingDescriptions()
	{
		VkVertexInputBindingDescription description;
		description.binding = 0;
		description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		description.stride = m_ShaderData.Stages[ShaderType::Vertex].Stride;
		return description;
	}
	std::vector<VkVertexInputAttributeDescription> VulkanShader::GetAttriDescriptions()
	{
		auto& data = m_ShaderData.Stages[ShaderType::Vertex].Inputs;
		std::vector<VkVertexInputAttributeDescription> descriptions(data.size());

		for (uint32_t i = 0; i < data.size(); i++) {
			auto& input = data[i];
			descriptions[i] = {};
			descriptions[i].binding = 0;
			descriptions[i].location = input.Location;
			descriptions[i].format = VKUtils::ShaderDataTypeToVkFormat(input.Type);
			descriptions[i].offset = input.Offset;
		}
		return descriptions;
	}
	std::vector<VkPipelineShaderStageCreateInfo> VulkanShader::GetStageInfo()
	{
		std::vector<VkPipelineShaderStageCreateInfo> info(m_ShaderCode.size());

		uint32_t i = 0;
		for (auto&& [stage, source] : m_ShaderCode) {
			info[i] = {};
			info[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			info[i].stage = stage;
			info[i].module = m_Modules[stage];
			info[i].pName = "main";
			i++;
		}
		return info;
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
		HZR_PROFILE_FUNCTION();
		std::filesystem::path cache = RenderEngine::GetShaderCompilePath();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

		if (false)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		m_ShaderCode.clear();

		for (auto&& [stage, source] : sources)
		{
			HZR_PROFILE_SCOPE("Shader stage");
			std::filesystem::path shaderFilePath = m_Path;
			std::filesystem::path cachedFilePath = cache / (shaderFilePath.filename().string() + Utils::ShaderStageCachedVulkanExtension(stage));

			VkShaderStageFlagBits vkStage = Utils::ShaderTypeToVkType(stage);

			if (File::Exists(cachedFilePath)) {
				if (!File::IsNewerThan(shaderFilePath, cachedFilePath)) {
					Timer timer;
					File::ReadBinaryFileUint32(cachedFilePath, m_ShaderCode[vkStage]);
					HZR_CORE_INFO("Reading VulkanShader binaries took {0} ms", timer.ElapsedMillis());
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
		HZR_PROFILE_FUNCTION();
		auto device = VulkanContext::GetDevice()->GetDevice();

		for (auto&& [stage, binary] : m_ShaderCode) {
			VkShaderModuleCreateInfo moduleInfo = {};
			moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleInfo.codeSize = 4 * binary.size();
			moduleInfo.pCode = reinterpret_cast<const uint32_t*>(binary.data());

			vkCreateShaderModule(device, &moduleInfo, nullptr, &m_Modules[stage]);
		}
	}
	void VulkanShader::Reflect()
	{
		HZR_PROFILE_FUNCTION();
		m_ShaderData.Stages.clear();

		for (auto&& [stage, binary] : m_ShaderCode) {

			spirv_cross::Compiler compiler(binary);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			ShaderStageData shaderStage = ProcessShaderStage(compiler, resources);
			m_ShaderData.Stages[Utils::ShaderTypeFromVkType(stage)] = shaderStage;

			for (auto& resource : resources.uniform_buffers)
			{
				bool found = false;
				for (auto& uniform : m_ShaderData.UniformsDescriptions) {
					if (uniform.Name == resource.name) {
						uniform.ShaderUsage |= VKUtils::ShaderTypeFromVulkanStage(stage);
						found = true;
						break;
					}
				}
				if (found) continue;

				auto& type = compiler.get_type(resource.base_type_id);

				ShaderUniformBufferDescription desc = {};
				desc.Name = resource.name;
				desc.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
				desc.MemberCount = type.member_types.size();
				desc.Size = compiler.get_declared_struct_size(type);
				desc.ShaderUsage |= VKUtils::ShaderTypeFromVulkanStage(stage);

				m_ShaderData.UniformsDescriptions.push_back(desc);
			}
		}

		for (auto& uniformDescription : m_ShaderData.UniformsDescriptions)
		{
			UniformBufferCreateInfo bufferInfo = {};
			bufferInfo.Name = uniformDescription.Name;
			bufferInfo.Binding = uniformDescription.Binding;
			bufferInfo.Size = uniformDescription.Size;
			bufferInfo.Usage = uniformDescription.ShaderUsage;

			m_UniformBuffers[bufferInfo.Name] = UniformBuffer::Create(&bufferInfo);
		}
		Rendering::Utils::PrintReflectResults(m_Path, m_ShaderData);
	}
	void VulkanShader::CreateDescriptorSet(VkDescriptorSetLayout* layout)
	{
		auto device = VulkanContext::GetDevice()->GetDevice();

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = VulkanContext::GetDevice()->GetDescriptorPool();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = layout;

		auto result = vkAllocateDescriptorSets(device, &allocInfo, &m_DescriptorSet);

		for (auto& [name, buffer] : m_UniformBuffers) {

			Ref<VulkanUniformBuffer> buf = buffer.As<VulkanUniformBuffer>();

			VkDescriptorBufferInfo descriptorBufferInfo = {};
			descriptorBufferInfo.buffer = buf->GetBuffer();
			descriptorBufferInfo.offset = 0;
			descriptorBufferInfo.range = buf->GetSize();

			VkWriteDescriptorSet descriptorWrite = {};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSet;
			descriptorWrite.dstBinding = buf->GetBinding();
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &descriptorBufferInfo;

			vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
		}
	}
}
