#pragma once

#include "Hazard/Rendering/Shader.h"
#include "vulkan/vulkan.h"

namespace Hazard::Rendering::Vulkan
{
	class VulkanShader : public Shader {
	public:
		VulkanShader(const std::string& file);
		~VulkanShader();

		void Reload() override;
		void Bind() override;
		void Unbind() override;

		VkShaderModule GetModule(VkShaderStageFlagBits index) { return m_Modules[index]; }

		VkVertexInputBindingDescription GetBindingDescriptions();
		std::vector<VkVertexInputAttributeDescription> GetAttriDescriptions();

		std::unordered_map<ShaderType, ShaderStageData> GetShaderData() { return m_ShaderStageData; };
		std::vector<VkPipelineShaderStageCreateInfo> GetStageInfo();
		void DestroyModules();

	private:

		void CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources);
		void CreateModules();
		void Reflect();

		std::string m_Path;
		std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>> m_ShaderCode;
		std::unordered_map<VkShaderStageFlagBits, VkShaderModule> m_Modules;
		std::unordered_map<ShaderType, ShaderStageData> m_ShaderStageData;
	};
}

