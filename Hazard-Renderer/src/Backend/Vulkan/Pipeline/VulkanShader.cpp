
#include "VulkanShader.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Renderer.h"

#include "Backend/Core/Pipeline/ShaderFactory.h"
#include "../VulkanContext.h"
#include "../VkUtils.h"
#include "../VulkanShaderCompiler.h"
#include "VulkanUniformBuffer.h"

#include <sstream>
#include <glad/glad.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace HazardRenderer::Vulkan
{
	VulkanShader::VulkanShader(const std::string& filePath) : m_FilePath(filePath)
	{
		HZR_ASSERT(!filePath.empty(), "Shader path cannot be empty");
		Reload();
	}
	VulkanShader::~VulkanShader()
	{
		Renderer::SubmitResourceFree([modules = m_ShaderModules]() mutable {
			const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
			for (auto& shader : modules)
				vkDestroyShaderModule(device, shader, nullptr);
			});
	}
	void VulkanShader::Reload()
	{
		if (ShaderFactory::HasCachedShader(ShaderStage::Vertex, m_FilePath))
		{
			__debugbreak();
			return;
		}

		std::cout << m_FilePath << std::endl;
		std::unordered_map<ShaderStage, std::string> sources = ShaderFactory::GetShaderSources(m_FilePath);

		//Compile Vulkan shader
		VulkanShaderCompiler compiler;
		std::unordered_map<ShaderStage, std::vector<uint32_t>> binaries;

		for (auto& [stage, source] : sources)
		{
			double compilationTime = 0.0;
			std::vector<ShaderDefine> defines = { { "VULKAN_API" } };

			//Compile to Vulkan SPV
			CompileInfo compileInfoVulkan = {};
			compileInfoVulkan.Renderer = RenderAPI::Vulkan;
			compileInfoVulkan.Name = m_FilePath;
			compileInfoVulkan.Stage = stage;
			compileInfoVulkan.Source = source;
			compileInfoVulkan.Defines = defines;

			if (!compiler.Compile(&compileInfoVulkan))
			{
				std::cout << compiler.GetErrorMessage() << std::endl;
				__debugbreak();
				continue;
			}
			compilationTime += compiler.GetCompileTime();
			binaries[stage] = compiler.GetCompiledBinary();
		}
		Reflect(binaries);
		Ref<VulkanShader> instance = this;
		Renderer::Submit([instance]() mutable {
			instance->CreateShaderModules();
			instance->CreateDescriptorSetLayouts();
			});
	}
	bool VulkanShader::SetUniformBuffer(uint32_t set, uint32_t binding, void* data, uint32_t size)
	{
		auto& uniformBuffer = m_UniformBuffers[set][binding];
		if (!uniformBuffer) HZR_ASSERT(false, "");
		//HZR_ASSERT(uniformBuffer, "[VulkanShader]: UniformBuffer '{0}' does not exist", name);
		uniformBuffer->SetData(data, size);
		return true;
	}
	void VulkanShader::Set(uint32_t set, uint32_t binding, Ref<Image2D> image)
	{

	}
	void VulkanShader::Set(uint32_t set, uint32_t binding, Ref<UniformBuffer> uniformBuffer)
	{

	}
	void VulkanShader::Reload_RT(bool forceCompile)
	{

	}
	const std::vector<uint32_t>& VulkanShader::GetDynamicOffsets()
	{
		uint32_t i = 0;
		for (auto& [set, buffers] : m_UniformBuffers) {
			for (auto& [binding, uniformBuffer] : buffers)
			{
				m_DynamicOffsets[i] = uniformBuffer.As<VulkanUniformBuffer>()->GetOffset();
				i++;
			}
		}
		return m_DynamicOffsets;
	}
	void VulkanShader::Reflect(const std::unordered_map<ShaderStage, std::vector<uint32_t>>& binaries)
	{
		Timer timer;
		m_ShaderData.Stages.clear();

		VulkanShaderCompiler compiler;
		m_ShaderCode = binaries;
		m_ShaderData = compiler.GetShaderResources(binaries);


		uint32_t descriptorSets = 0;
		uint32_t size = 0;
		for (auto& [set, buffers] : m_ShaderData.UniformsDescriptions)
		{
			for (auto& [binding, buffer] : buffers)
			{
				UniformBufferCreateInfo info = {};
				info.Name = buffer.Name;
				info.Set = set;
				info.Binding = binding;
				info.Size = buffer.Size;
				info.Usage = buffer.UsageFlags;

				m_UniformBuffers[set][binding] = UniformBuffer::Create(&info);
				size++;
			}
			if (set > descriptorSets)
				descriptorSets = set;
		}
		for (auto& [set, buffer] : m_ShaderData.ImageSamplers)
		{
			if (set > descriptorSets)
				descriptorSets = set;
		}
		m_DynamicOffsets.resize(size);
		m_DescriptorSets.resize(descriptorSets + 1);

		VulkanShaderCompiler::PrintReflectionData(m_ShaderData);
		std::cout << "Reflection took: " << timer.ElapsedMillis() << "ms" << std::endl;
	}
	void VulkanShader::CreateShaderModules()
	{
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
		uint32_t index = 0;

		m_ShaderModules.resize(m_ShaderCode.size());

		for (auto& [stage, code] : m_ShaderCode) {

			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.size() * sizeof(uint32_t);
			createInfo.pCode = code.data();

			VK_CHECK_RESULT(vkCreateShaderModule(device, &createInfo, nullptr, &m_ShaderModules[index]), "Failed to create VkShaderModule");

			auto& stageInfo = m_ShaderStageCreateInfos.emplace_back();
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.module = m_ShaderModules[index];
			stageInfo.pName = "main";
			stageInfo.flags = 0;
			stageInfo.stage = (VkShaderStageFlagBits)VkUtils::GetVulkanShaderStage((uint32_t)stage);

			index++;
		}
	}
	void VulkanShader::CreateDescriptorSetLayouts()
	{
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		std::cout << m_FilePath << std::endl;

		m_DescriptorSetLayouts.resize(m_DescriptorSets.size());
		for (uint32_t set = 0; set < m_DescriptorSets.size(); set++)
		{
			std::vector<VkDescriptorSetLayoutBinding> bindings;
			auto& descriptorSet = m_DescriptorSets[set];

			for (auto& [binding, buffer] : m_ShaderData.UniformsDescriptions[set])
			{
				auto& descriptorBinding = bindings.emplace_back();
				descriptorBinding = {};
				descriptorBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				descriptorBinding.binding = buffer.Binding;
				descriptorBinding.descriptorCount = 1;
				descriptorBinding.stageFlags = VkUtils::GetVulkanShaderStage(buffer.UsageFlags);

				VkWriteDescriptorSet writeDescriptor = {};
				writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeDescriptor.dstBinding = descriptorBinding.binding;
				writeDescriptor.descriptorType = descriptorBinding.descriptorType;
				writeDescriptor.dstArrayElement = 0;
				writeDescriptor.descriptorCount = 1;

				m_WriteDescriptorSets[set][buffer.Binding] = writeDescriptor;
			}

			for (auto& [binding, sampler] : m_ShaderData.ImageSamplers[set])
			{
				auto& descriptorBinding = bindings.emplace_back();
				descriptorBinding = {};
				descriptorBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				descriptorBinding.binding = binding;
				descriptorBinding.descriptorCount = sampler.ArraySize;
				descriptorBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			}

			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = (uint32_t)bindings.size();
			layoutInfo.pBindings = bindings.data();

			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_DescriptorSetLayouts[set]), "Failed to create descriptor layout");

			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &m_DescriptorSetLayouts[set];
			m_DescriptorSets[set] = VulkanContext::GetInstance()->RT_AllocateDescriptorSet(allocInfo);

			auto& dstSet = m_WriteDescriptorSets[set];

			for (auto& [binding, writeDescriptor] : dstSet)
			{
				auto& uniformBuffer = m_UniformBuffers[set][binding].As<VulkanUniformBuffer>();

				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = uniformBuffer->GetVulkanBuffer();
				bufferInfo.offset = 0;
				bufferInfo.range = uniformBuffer->GetSize();

				auto& writeSet = writeDescriptor;
				writeSet.dstSet = m_DescriptorSets[set];
				writeSet.pBufferInfo = &bufferInfo;
				std::cout << "Binding: Set: " << set << " -> " << binding << ", buffer " << writeDescriptor.pBufferInfo->buffer << std::endl;
				vkUpdateDescriptorSets(device, 1, &writeSet, 0, nullptr);
			}
		}
	}
	void VulkanShader::CreatePushConstantRanges()
	{

	}
}
#endif
