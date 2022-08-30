
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
	bool VulkanShader::SetUniformBuffer(const std::string& name, void* data, uint32_t size)
	{
		auto& uniformBuffer = m_UniformBuffers[name];
		if (!uniformBuffer) return false;
		//HZR_ASSERT(uniformBuffer, "[VulkanShader]: UniformBuffer '{0}' does not exist", name);
		uniformBuffer->SetData(data, size);
		return true;
	}
	void VulkanShader::Reload_RT(bool forceCompile)
	{

	}
	const std::vector<uint32_t>& VulkanShader::GetDynamicOffsets()
	{
		uint32_t i = 0;
		for (auto& [binding, uniformBuffer] : m_UniformBuffers)
		{
			m_DynamicOffsets[i] = 0; // uniformBuffer.As<VulkanUniformBuffer>()->GetOffset();
			i++;
		}
		return m_DynamicOffsets;
	}
	void VulkanShader::Reflect(const std::unordered_map<ShaderStage, std::vector<uint32_t>> binaries)
	{
		Timer timer;
		m_ShaderData.Stages.clear();

		VulkanShaderCompiler compiler;
		uint32_t uniformCount = 0;
		m_ShaderCode = binaries;
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

		m_TypeCounts.clear();

		for (uint32_t set = 0; set < m_DescriptorSets.size(); set++)
		{
			auto& descritorSet = m_DescriptorSets[set];
			if (m_ShaderData.UniformsDescriptions.size() > 0)
			{
				VkDescriptorPoolSize& typeCount = m_TypeCounts[set].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				typeCount.descriptorCount = (uint32_t)m_ShaderData.UniformsDescriptions.size();
			}
			if (m_ShaderData.ImageSamplers.size() > 0)
			{
				VkDescriptorPoolSize& typeCount = m_TypeCounts[set].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				typeCount.descriptorCount = (uint32_t)m_ShaderData.ImageSamplers.size();
			}

			std::vector<VkDescriptorSetLayoutBinding> bindings;


			for (auto& [binding, uniformBuffer] : m_ShaderData.UniformsDescriptions)
			{
				VkDescriptorSetLayoutBinding& layout = bindings.emplace_back();
				layout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				layout.binding = binding;
				layout.descriptorCount = 1;
				layout.pImmutableSamplers = nullptr;
				//layout.stageFlags = VkUtils::GetVulkanShaderStage(uniformBuffer);

				//VkWriteDescriptorSet& writeSet = m_WriteDescriptorSets[uniformBuffer.Name];
				//writeSet = {};
				//writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				//writeSet.descriptorType = layout.descriptorType;
				//writeSet.descriptorCount = 1;
				//writeSet.dstBinding = binding;
			}

			if (m_ShaderData.Stages.find(ShaderStage::Fragment) != m_ShaderData.Stages.end())
			{
				for (auto& [binding, sampler] : m_ShaderData.ImageSamplers)
				{
					/*
					VkDescriptorSetLayoutBinding& layout = bindings.emplace_back();
					layout.binding = binding;
					layout.descriptorCount = sampler;
					layout.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					layout.pImmutableSamplers = nullptr;
					layout.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

					VkWriteDescriptorSet& writeSet = m_WriteDescriptorSets[sampler.Name];
					writeSet = {};
					writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					writeSet.descriptorType = layout.descriptorType;
					writeSet.descriptorCount = sampler.ArraySize;
					writeSet.dstBinding = binding;
					*/
				}
			}

			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = (uint32_t)bindings.size();
			layoutInfo.pBindings = bindings.data();

			if (set >= m_DescriptorSetLayouts.size())
				m_DescriptorSetLayouts.resize((uint32_t)set + 1);

			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_DescriptorSetLayouts[set]), "Failed to create DescriptorSetLayout");

			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &m_DescriptorSetLayouts[set];

			m_DescriptorSets[set] = VulkanContext::GetInstance()->RT_AllocateDescriptorSet(allocInfo);

			for (auto& [binding, uniformBuffer] : m_UniformBuffers)
			{
				auto& vulkanUniformBuffer = uniformBuffer.As<VulkanUniformBuffer>();
				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = vulkanUniformBuffer->GetVulkanBuffer();
				bufferInfo.offset = 0;
				bufferInfo.range = vulkanUniformBuffer->GetWriteBuffer().Size;

				auto& writeDescriptorSet = m_WriteDescriptorSets[uniformBuffer->GetName()];
				writeDescriptorSet.dstSet = m_DescriptorSets[set];
				writeDescriptorSet.dstArrayElement = 0;
				writeDescriptorSet.pBufferInfo = &bufferInfo;

				vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
			}
		}
	}
	void VulkanShader::CreatePushConstantRanges()
	{

	}
}
#endif
