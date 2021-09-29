#pragma once

#include "Hazard/Rendering/Shader.h"
#include "Hazard/Rendering/Buffers/Buffers.h"
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
		void SetUniformBuffer(const std::string& name, void* data) override;

		VkResult CreateUniformDescriptorLayout(VkDescriptorSetLayout* layout);

		VkVertexInputBindingDescription GetBindingDescriptions();
		std::vector<VkVertexInputAttributeDescription> GetAttriDescriptions();

		const ShaderData& GetShaderData() override { return m_ShaderData; };
		std::vector<VkPipelineShaderStageCreateInfo> GetStageInfo();

		void DestroyModules();

	private:

		void CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources);
		void CreateModules();
		void Reflect();

		std::string m_Path;
		std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>> m_ShaderCode;
		std::unordered_map<VkShaderStageFlagBits, VkShaderModule> m_Modules;
		ShaderData m_ShaderData;

		std::unordered_map<std::string, Ref<UniformBuffer>> m_UniformBuffers;
	};
}

