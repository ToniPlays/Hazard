
#include "VulkanShader.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Renderer.h"

#include "../VulkanContext.h"
#include "../VkUtils.h"
#include "../Textures/VulkanImage2D.h"
#include "../Textures/VulkanCubemapTexture.h"
#include "../RTCore/VulkanTopLevelAS.h"
#include "Profiling/Timer.h"

#include "Backend/Core/ShaderCompiler.h"
#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::Vulkan
{
	VulkanShader::VulkanShader(const std::vector<ShaderStageCode>& shaderCode)
	{
		HZR_PROFILE_FUNCTION();

		for (auto& code : shaderCode)
			m_ShaderCode[code.Stage] = Buffer::Copy(code.ShaderCode);

		Reload();
	}
	VulkanShader::~VulkanShader()
	{
		HZR_PROFILE_FUNCTION();
		Renderer::SubmitResourceFree([modules = m_ShaderModules]() mutable {
			const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
		for (auto& shader : modules)
			vkDestroyShaderModule(device, shader, nullptr);
			});
	}
	void VulkanShader::Reload()
	{
		HZR_PROFILE_FUNCTION();
		Timer timer;

		Reflect();

		Ref<VulkanShader> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->CreateShaderModules();
			instance->CreateDescriptorSetLayouts();
			});
	}

	void VulkanShader::Reload_RT(bool forceCompile)
	{
		HZR_PROFILE_FUNCTION();
	}
	void VulkanShader::Reflect()
	{
		HZR_PROFILE_FUNCTION();

		Timer timer;

		m_ShaderData.Stages.clear();
		m_ShaderData = ShaderCompiler::GetShaderResources(m_ShaderCode);

		uint32_t descriptorSets = 0;
		uint32_t size = 0;

		for (auto& [set, buffer] : m_ShaderData.ImageSamplers)
		{
			if (set > descriptorSets)
				descriptorSets = set;
		}

		m_DynamicOffsets.resize(size);
	}
	void VulkanShader::CreateShaderModules()
	{
		HZR_PROFILE_FUNCTION();
		const auto& device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
		uint32_t index = 0;

		m_ShaderModules.resize(m_ShaderCode.size());

		for (auto& [stage, code] : m_ShaderCode) {

			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.Size;
			createInfo.pCode = (uint32_t*)code.Data;

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
		HZR_PROFILE_FUNCTION();
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		for (uint32_t set = 0; set < 3; set++)
		{
			std::vector< VkDescriptorSetLayoutBinding> bindings;

			for (auto& [binding, accel] : m_ShaderData.AccelerationStructures[set])
			{
				VkDescriptorSetLayoutBinding& descriptorBinding = bindings.emplace_back();
				descriptorBinding = {};
				descriptorBinding.descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
				descriptorBinding.binding = accel.Location;
				descriptorBinding.descriptorCount = 1;
				descriptorBinding.stageFlags = VkUtils::GetVulkanShaderStage((uint32_t)accel.UsageFlags);
				descriptorBinding.stageFlags = VK_SHADER_STAGE_ALL;
			}
			for (auto& [binding, buffer] : m_ShaderData.StorageBuffers[set])
			{
				VkDescriptorSetLayoutBinding& descriptorBinding = bindings.emplace_back();
				descriptorBinding = {};
				descriptorBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				descriptorBinding.binding = buffer.Location;
				descriptorBinding.descriptorCount = 1;
				descriptorBinding.stageFlags = VkUtils::GetVulkanShaderStage((uint32_t)buffer.UsageFlags);
				descriptorBinding.stageFlags = VK_SHADER_STAGE_ALL;

			}
			for (auto& [binding, buffer] : m_ShaderData.UniformsDescriptions[set])
			{
				VkDescriptorSetLayoutBinding& descriptorBinding = bindings.emplace_back();
				descriptorBinding = {};
				descriptorBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				descriptorBinding.binding = binding;
				descriptorBinding.descriptorCount = 1;
				descriptorBinding.stageFlags = VkUtils::GetVulkanShaderStage(buffer.UsageFlags);
				descriptorBinding.stageFlags = VK_SHADER_STAGE_ALL;
			}
			for (auto& [binding, sampler] : m_ShaderData.ImageSamplers[set])
			{
				VkDescriptorSetLayoutBinding& descriptorBinding = bindings.emplace_back();
				descriptorBinding = {};
				descriptorBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				descriptorBinding.binding = binding;
				descriptorBinding.descriptorCount = sampler.ArraySize;
				descriptorBinding.stageFlags = VkUtils::GetVulkanShaderStage(sampler.Flags);
				descriptorBinding.stageFlags = VK_SHADER_STAGE_ALL;
			}
			for (auto& [binding, storageImage] : m_ShaderData.StorageImages[set])
			{
				VkDescriptorSetLayoutBinding& descriptorBinding = bindings.emplace_back();
				descriptorBinding = {};
				descriptorBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				descriptorBinding.binding = binding;
				descriptorBinding.descriptorCount = storageImage.ArraySize;
				descriptorBinding.stageFlags = VkUtils::GetVulkanShaderStage(storageImage.Flags);
				descriptorBinding.stageFlags = VK_SHADER_STAGE_ALL;
			}

			for (auto& [binding, range] : m_ShaderData.PushConstants[set])
			{
				auto& constant = m_PushConstantRanges[range.Name];
				constant.Stages = VkUtils::GetVulkanShaderStage(range.UsageFlags);
				constant.Size = range.Size;
			}

			if (bindings.size() == 0) continue;

			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = (uint32_t)bindings.size();
			layoutInfo.pBindings = bindings.data();

			auto& layout = m_DescriptorSetLayouts.emplace_back();

			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout), "Failed to create descriptor layout");
			VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, fmt::format("{} layout {}", "Shader", set), layout);
		}
	}
}
#endif
