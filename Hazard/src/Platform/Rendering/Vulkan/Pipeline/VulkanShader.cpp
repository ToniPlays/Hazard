#pragma once

#include <hzrpch.h>
#include "VulkanShader.h"
#include "../VulkanContext.h"
#include "../VKUtils.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "../VulkanTexture2D.h"
#include "VulkanBuffers.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Hazard::Rendering::Vulkan
{
	namespace Utils
	{
		static VkShaderStageFlagBits ShaderTypeToVkType(ShaderType type) {
			switch (type)
			{
			case ShaderType::Vertex:	return VK_SHADER_STAGE_VERTEX_BIT;
			case ShaderType::Fragment:	return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderType::Compute:	return VK_SHADER_STAGE_COMPUTE_BIT;
			case ShaderType::Geometry:	return VK_SHADER_STAGE_GEOMETRY_BIT;
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
			case VK_SHADER_STAGE_VERTEX_BIT:	return ShaderType::Vertex;
			case VK_SHADER_STAGE_FRAGMENT_BIT:	return ShaderType::Fragment;
			case VK_SHADER_STAGE_COMPUTE_BIT:	return ShaderType::Compute;
			case VK_SHADER_STAGE_GEOMETRY_BIT:	return ShaderType::Geometry;
			}
			HZR_CORE_ASSERT(false, "[VulkanShader]: Undefined Vulkan ShaderStageFlags {0}", VkTypeToString(type));
			return ShaderType::None;
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
		auto shaderSources = ShaderFactory::GetShaderSources(m_Path);

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
	void VulkanShader::Set(const std::string& name, uint32_t index, Ref<Texture2D>& value)
	{
		if (!value) return;

		for (auto& [binding, sampler] : m_ShaderData.Stages[ShaderType::Fragment].SampledImages)
		{
			if (strcmp(sampler.Name.c_str(), name.c_str()) != 0) continue;

			Ref<VulkanImage2D> image = value.As<VulkanTexture2D>()->GetImage();
			if (!image) return;

			VkDescriptorImageInfo imageInfo = image->GetDescriptor();

			auto device = VulkanContext::GetDevice()->GetDevice();
			VkWriteDescriptorSet descriptorWrite = {};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSet;
			descriptorWrite.dstBinding = sampler.Binding;
			descriptorWrite.dstArrayElement = index;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
			break;
		}
	}
	VkResult VulkanShader::CreateDescriptorLayout(VkDescriptorSetLayout* layout)
	{
		auto device = VulkanContext::GetDevice()->GetDevice();
		std::vector<VkDescriptorSetLayoutBinding> bindings;

		for (auto& [binding, uniform] : m_ShaderData.UniformsDescriptions)
		{
			VkDescriptorSetLayoutBinding& layoutBinding = bindings.emplace_back();
			layoutBinding.binding = binding;
			layoutBinding.descriptorCount = 1;
			layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			layoutBinding.pImmutableSamplers = nullptr;
			layoutBinding.stageFlags = VKUtils::ShaderUsageToVulkanUsage(uniform.ShaderUsage);
		}

		if (m_ShaderData.Stages.find(ShaderType::Fragment) != m_ShaderData.Stages.end()) {
			auto& fragStage = m_ShaderData.Stages[ShaderType::Fragment];

			for (auto& [binding, sampler] : fragStage.SampledImages) {

				VkDescriptorSetLayoutBinding& samplerLayoutBinding = bindings.emplace_back();
				samplerLayoutBinding.binding = sampler.Binding;
				samplerLayoutBinding.descriptorCount = sampler.ArraySize;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr;
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

			}
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = (uint32_t)bindings.size();
		layoutInfo.pBindings = bindings.data();

		return vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, layout);
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
		for (auto& [stage, source] : m_ShaderCode) {
			info[i] = {};
			info[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			info[i].stage = Utils::ShaderTypeToVkType(stage);
			info[i].module = m_Modules[Utils::ShaderTypeToVkType(stage)];
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
		m_ShaderCode.clear();

		for (auto&& [stage, source] : sources)
		{
			HZR_PROFILE_SCOPE("Shader stage");

			VkShaderStageFlagBits vkStage = Utils::ShaderTypeToVkType(stage);
			auto& binaries = ShaderFactory::GetShaderBinaries(m_Path, stage, RenderAPI::Vulkan);

			if (binaries.size() > 0) {
				m_ShaderCode[stage] = binaries;
				continue;
			}

			CompileInfo compileInfo = {};
			compileInfo.Path = m_Path;
			compileInfo.Environment = RenderAPI::Vulkan;
			compileInfo.Optimization = Optimization::None;
			compileInfo.Stage = stage;
			compileInfo.Source = source;

			ShaderFactory::Compile(&compileInfo);

			if (!compileInfo.Succeeded()) {
				HZR_CORE_ERROR(compileInfo.Error);
				auto& binaries = ShaderFactory::GetShaderBinaries(m_Path, stage, RenderAPI::Vulkan);
				m_ShaderCode[stage] = binaries;
				continue;
			}

			m_ShaderCode[stage] = compileInfo.Binary;
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

			vkCreateShaderModule(device, &moduleInfo, nullptr, &m_Modules[Utils::ShaderTypeToVkType(stage)]);
		}
	}
	void VulkanShader::Reflect()
	{
		HZR_PROFILE_FUNCTION();
		m_ShaderData.Stages.clear();

		for (auto&& [stage, binary] : m_ShaderCode) {

			spirv_cross::Compiler compiler(binary);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();
			ShaderStageData shaderStage = ShaderFactory::GetShaderResources(binary);
			m_ShaderData.Stages[stage] = shaderStage;

			for (auto& resource : resources.uniform_buffers)
			{
				auto& type = compiler.get_type(resource.base_type_id);

				ShaderUniformBufferDescription desc = {};
				desc.Name = resource.name;
				desc.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
				desc.MemberCount = type.member_types.size();
				desc.Size = compiler.get_declared_struct_size(type);
				desc.ShaderUsage |= (uint32_t)stage;

				auto it = m_UniformBuffers.find(desc.Name);
				if (it != m_UniformBuffers.end())
				{
					auto& buffer = m_UniformBuffers[desc.Name];

					HZR_CORE_ASSERT(desc.Binding == buffer->GetBinding(), "OpenGL UniformBuffer Binding missmatch for name: {0}", desc.Name);
					continue;
				}

				//TODO: Maybe use binding as key?
				m_ShaderData.UniformsDescriptions[desc.Binding] = desc;

				UniformBufferCreateInfo bufferInfo = {};
				bufferInfo.Name = desc.Name;
				bufferInfo.Binding = desc.Binding;
				bufferInfo.Size = desc.Size;

				m_UniformBuffers[bufferInfo.Name] = UniformBuffer::Create(&bufferInfo);
			}
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