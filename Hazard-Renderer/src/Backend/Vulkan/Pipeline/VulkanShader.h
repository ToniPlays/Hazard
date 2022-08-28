#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Pipeline/Shader.h"
#include "Backend/Core/Pipeline/Buffers.h"
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	struct PushConstantRange 
	{
		VkShaderStageFlagBits Stages = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		uint32_t Offset = 0;
		uint32_t Size = 0;
	};

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const std::string& filePath);
		~VulkanShader();

		void Reload() override;
		bool SetUniformBuffer(const std::string& name, void* data, uint32_t size) override;
		void Set(const std::string& name, uint32_t index, uint32_t value) override;
		void Set(const std::string& name, uint32_t index, Ref<Image2D> value) override;
		UniformBuffer& GetUniform(const std::string& name) override { return *m_UniformBuffers[name]; };

		const ShaderData& GetShaderData() { return m_ShaderData; };

		//Vulkan specific

		std::vector<VkDescriptorSetLayout>& GetAllDescriptorSetLayouts() { return m_DescriptorSetLayouts; }
		std::vector<PushConstantRange>& GetPushConstantRanges() { return m_PushConstantRanges; }
		std::vector<VkPipelineShaderStageCreateInfo> GetPipelineShaderStageCreateInfos() { return m_ShaderStageCreateInfos; }

	private:

		bool CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources);
		void Reflect();

		std::string m_FilePath;

		std::unordered_map<ShaderType, std::vector<uint32_t>> m_VulkanSPIRV;

		ShaderData m_ShaderData;
		std::unordered_map<std::string, Ref<UniformBuffer>> m_UniformBuffers;

		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStageCreateInfos;
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
		std::vector<PushConstantRange> m_PushConstantRanges;
	};
}
#endif
