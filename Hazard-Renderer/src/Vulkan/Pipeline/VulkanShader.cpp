
#include "VulkanShader.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Core/Renderer.h"

#include "../VulkanContext.h"
#include "../VkUtils.h"
#include "../Textures/VulkanImage2D.h"
#include "../Textures/VulkanCubemapTexture.h"
#include "../RTCore/VulkanTopLevelAS.h"
#include "Profiling/Timer.h"

#include "Core/ShaderCompiler.h"

#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::Vulkan
{
	VulkanShader::VulkanShader(const std::unordered_map<uint32_t, std::string>& shaderModules)
	{
		HZR_PROFILE_FUNCTION();

		for (auto& [stage, code] : shaderModules)
			m_ShaderCode[stage] = Buffer::Copy(code.c_str(), code.length());

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

		Ref<VulkanShader> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->CreateShaderModules();
			});
	}

	void VulkanShader::Reload_RT(bool forceCompile)
	{
		HZR_PROFILE_FUNCTION();
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
}
#endif