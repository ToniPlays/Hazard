#pragma once

#include <hzrpch.h>
#include "VulkanShader.h"
#include "../VulkanContext.h"
#include "../VKUtils.h"
#include "../VulkanTexture2D.h"
#include "VulkanBuffers.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Rendering/Loaders/ShaderFactory.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

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
		Reload();
	}
	VulkanShader::~VulkanShader()
	{
		HZR_CORE_WARN("Destroyed shader {0}", m_Path);
		m_UniformBuffers.clear();
		m_UnformBufferBindings.clear();
		m_DescriptorSets.clear();
		m_WriteDescriptors.clear();

		auto modules = m_Modules;
		RenderContextCommand::SubmitResourceFree([modules, layout = m_Layout]() mutable {
			HZR_CORE_WARN("Deleting shader modules");
			auto device = VulkanContext::GetDevice()->GetDevice();

			for (auto&& [stage, module] : modules) {
				vkDestroyShaderModule(device, module, nullptr);
			}
			modules.clear();
			//vkDestroyDescriptorSetLayout(device, layout, nullptr);
			});
	}
	void VulkanShader::Reload()
	{
		Timer timer;
		auto shaderSources = ShaderFactory::GetShaderSources(m_Path);

		CompileOrGetVulkanBinaries(shaderSources);
		Reflect();
		CreateModules();
		CreateDescriptorLayout();
		CreateDescriptorSets();

		HZR_CORE_TRACE("[VulkanShader]: Creation took {0} ms", timer.ElapsedMillis());
	}
	void VulkanShader::Bind(Ref<RenderCommandBuffer> cmdBufer)
	{
		for (auto& [name, buffer] : m_UniformBuffers) {
			buffer->Bind(cmdBufer);
		}
	}
	void VulkanShader::Unbind()
	{

	}
	void VulkanShader::SetUniformBuffer(const std::string& name, void* data, uint32_t size)
	{
		auto& uniformBuffer = m_UniformBuffers[m_UnformBufferBindings[name]];
		HZR_CORE_ASSERT(uniformBuffer, "[VulkanShader]: UniformBuffer '{0}' does not exist", name);
		uniformBuffer->SetData(data, size);
	}
	void VulkanShader::Set(const std::string& name, uint32_t index, Ref<Texture2D>& value)
	{
		if (!value) return;

		for (auto& [binding, sampler] : m_ShaderData.Stages[ShaderType::Fragment].SampledImages)
		{
			if (strcmp(sampler.Name.c_str(), name.c_str()) != 0) continue;

			Ref<VulkanImage2D> image = value.As<VulkanTexture2D>()->GetImage();
			if (!image) return;

			ShaderSampledImage samplerInstance = sampler;
			Ref<VulkanShader> instance = this;

			RenderContextCommand::SubmitResourceCreate([image, samplerInstance, index, sets = m_DescriptorSets]() mutable {
				VkDescriptorImageInfo imageInfo = image->GetDescriptor();

				auto device = VulkanContext::GetDevice()->GetDevice();
				VkWriteDescriptorSet descriptorWrite = {};
				descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite.dstSet = sets[VulkanContext::GetSwapchain()->GetCurrentBufferIndex()];
				descriptorWrite.dstBinding = samplerInstance.Binding;
				descriptorWrite.dstArrayElement = index;
				descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				descriptorWrite.descriptorCount = 1;
				descriptorWrite.pImageInfo = &imageInfo;

				vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
				});
			break;
		}
	}
	VkResult VulkanShader::CreateDescriptorLayout()
	{
		auto device = VulkanContext::GetDevice()->GetDevice();
		std::vector<VkDescriptorSetLayoutBinding> bindings;

		for (auto& [binding, uniform] : m_ShaderData.UniformsDescriptions)
		{
			VkDescriptorSetLayoutBinding& layoutBinding = bindings.emplace_back();
			layoutBinding.binding = binding;
			layoutBinding.descriptorCount = 1;
			layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
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

		return vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_Layout);
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
	std::vector<uint32_t> VulkanShader::GetDynamicOffsets()
	{
		for (auto& [binding, buffer] : m_UniformBuffers) {
			m_DynamicOffsets[binding] = buffer.As<VulkanUniformBuffer>()->m_Offset;
		}
		return m_DynamicOffsets;
	}
	void VulkanShader::CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources)
	{
		m_ShaderCode.clear();

		for (auto&& [stage, source] : sources)
		{
			HZR_PROFILE_SCOPE("Shader stage");

			VkShaderStageFlagBits vkStage = Utils::ShaderTypeToVkType(stage);
			const auto& binaries = ShaderFactory::GetShaderBinaries(m_Path, stage, RenderAPI::Vulkan);

			if (binaries.size() > 0 && false) {
				m_ShaderCode[stage] = binaries;
				continue;
			}

			std::vector<ShaderDefine> defines = { { "RENDERER_VULKAN" } };

			CompileInfo compileInfo = {};
			compileInfo.Path = m_Path;
			compileInfo.Environment = RenderAPI::Vulkan;
			compileInfo.Optimization = Optimization::None;
			compileInfo.Stage = stage;
			compileInfo.Source = source;
			compileInfo.Defines = defines;

			ShaderFactory::Compile(&compileInfo);

			if (!compileInfo.Succeeded()) {
				HZR_CORE_ERROR(compileInfo.Error);
				const auto& binaries = ShaderFactory::GetShaderBinaries(m_Path, stage, RenderAPI::Vulkan);
				m_ShaderCode[stage] = binaries;
				continue;
			}
			m_ShaderCode[stage] = compileInfo.Binary;
		}
	}
	void VulkanShader::CreateModules()
	{
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
		m_ShaderData.Stages.clear();

		for (auto&& [stage, binary] : m_ShaderCode) {

			spirv_cross::Compiler compiler(binary);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();
			ShaderStageData shaderStage = ShaderFactory::GetShaderResources(binary);
			m_ShaderData.Stages[stage] = shaderStage;

			for (auto resource : resources.uniform_buffers)
			{
				auto& type = compiler.get_type(resource.base_type_id);

				uint32_t size = compiler.get_declared_struct_size(type);

				ShaderUniformBufferDescription spec = {};
				spec.Name = resource.name;
				spec.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
				spec.MemberCount = type.member_types.size();
				spec.Size = size = Math::Max<float>(256, spec.Size);
				spec.ShaderUsage |= (uint32_t)stage;

				auto it = m_ShaderData.UniformsDescriptions.find(spec.Binding);
				if (it != m_ShaderData.UniformsDescriptions.end())
				{
					auto& buffer = m_ShaderData.UniformsDescriptions[spec.Binding];
					buffer.ShaderUsage |= (uint32_t)stage;
					continue;
				}

				m_ShaderData.UniformsDescriptions[spec.Binding] = spec;
			}
		}
		for (auto& [binding, spec] : m_ShaderData.UniformsDescriptions) {
			UniformBufferCreateInfo bufferInfo = {};
			bufferInfo.Name = spec.Name;
			bufferInfo.Binding = spec.Binding;
			bufferInfo.Size = Math::Max<float>(256, spec.Size);
			bufferInfo.Usage = spec.ShaderUsage;
			bufferInfo.IsShared = true;

			Ref<UniformBuffer> buffer = UniformBuffer::Create(&bufferInfo);
			m_UniformBuffers[bufferInfo.Binding] = buffer;
			m_UnformBufferBindings[bufferInfo.Name] = bufferInfo.Binding;
		}

		m_DynamicOffsets.resize(m_UniformBuffers.size());
		//Rendering::Utils::PrintReflectResults(m_Path, m_ShaderData);
	}
	VkResult VulkanShader::CreateDescriptorSets()
	{
		auto device = VulkanContext::GetDevice()->GetDevice();
		uint32_t framesInFlight = RenderContextCommand::GetImagesInFlight();
		m_DescriptorSets.resize(framesInFlight);

		for (uint32_t i = 0; i < framesInFlight; i++) {
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = VulkanContext::GetDevice()->GetDescriptorPool(i);
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &m_Layout;

			auto result = vkAllocateDescriptorSets(device, &allocInfo, &m_DescriptorSets[i]);
		}
		
		//TODO: Flip this to reduce n buffer updates
		for (uint32_t i = 0; i < framesInFlight; i++) {

			for (auto& [name, buffer] : m_UniformBuffers) {
				UpdateBufferDescriptor(buffer.As<VulkanUniformBuffer>(), i);
			}
		}
		return VK_SUCCESS;
	}
	void VulkanShader::UpdateBufferDescriptor(Ref<VulkanUniformBuffer> buffer, uint32_t index)
	{
		auto device = VulkanContext::GetDevice()->GetDevice();
		Ref<VulkanUniformBuffer> buf = buffer.As<VulkanUniformBuffer>();

		VkDescriptorBufferInfo descriptorBufferInfo = {};
		descriptorBufferInfo.buffer = buf->GetBuffer();
		descriptorBufferInfo.offset = 0;
		descriptorBufferInfo.range = buf->GetSize();

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_DescriptorSets[index];
		descriptorWrite.dstBinding = buf->GetBinding();
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &descriptorBufferInfo;

		vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
	}
}