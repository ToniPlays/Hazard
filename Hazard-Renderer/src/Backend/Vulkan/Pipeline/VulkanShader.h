#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include <map>

#include "Backend/Core/Shader.h"
#include "Backend/Core/GPUBuffer.h"
#include "Backend/Core/Texture.h"
#include "VulkanDescriptorSet.h"
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	class VulkanImage2D;
	struct PushConstantRange 
	{
		VkShaderStageFlags Stages = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		uint32_t Offset = 0;
		uint32_t Size = 0;
	};

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const std::vector<ShaderStageCode>& shaderCode);
		~VulkanShader();

		void Reload() override;
		const ShaderData& GetShaderData() { return m_ShaderData; };
		std::unordered_map<uint32_t, Buffer> GetShaderCode() const override { return m_ShaderCode; };
		
		//Vulkan specific
		void Reload_RT(bool forceCompile = false);

		const std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts() const { return m_DescriptorSetLayouts; };
		std::unordered_map<std::string, PushConstantRange>& GetPushConstantRanges() { return m_PushConstantRanges; }
		std::vector<VkPipelineShaderStageCreateInfo> GetPipelineShaderStageCreateInfos() { return m_ShaderStageCreateInfos; }

	private:
		void Reflect();
		void CreateShaderModules();
		void CreateDescriptorSetLayouts();

		std::unordered_map<uint32_t, Buffer> m_ShaderCode;

		ShaderData m_ShaderData;

		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStageCreateInfos;
		std::unordered_map<std::string, PushConstantRange> m_PushConstantRanges;

		std::vector<VkShaderModule> m_ShaderModules;
		std::vector<uint32_t> m_DynamicOffsets;
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
	};
}
#endif
