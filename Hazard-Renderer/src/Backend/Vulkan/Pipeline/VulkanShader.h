#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include <map>

#include "Backend/Core/Pipeline/Shader.h"
#include "Backend/Core/Pipeline/Buffers.h"
#include "Backend/Core/Texture/Texture.h"
#include "VulkanDescriptorSet.h"
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	class VulkanImage2D;
	struct PushConstantRange 
	{
		VkShaderStageFlagBits Stages = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		uint32_t Offset = 0;
		uint32_t Size = 0;
	};

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const std::vector<ShaderStageCode>& shaderCode);
		~VulkanShader();

		void Reload() override;
		void Set(const std::string& name, uint32_t index, Ref<Image2D> image) override;
		void Set(const std::string& name, uint32_t index, Ref<CubemapTexture> cubemap) override;
		void Set(const std::string& name, uint32_t index, Ref<AccelerationStructure> accelerationStructure) override;
		void Set(const std::string& name, uint32_t index, Ref<BufferBase> buffer) override;

		const ShaderData& GetShaderData() { return m_ShaderData; };
		std::unordered_map<ShaderStage, Buffer> GetShaderCode() const override { return m_ShaderCode; };
		
		//Vulkan specific
		void Reload_RT(bool forceCompile = false);

		std::vector<VkDescriptorSetLayout> GetAllDescriptorSetLayouts();
		VkDescriptorSetLayout& GetDescriptorSetLayout(uint32_t index);
		std::vector<VulkanDescriptorSet>& GetDescriptorSets();
		std::vector<VkDescriptorSet> GetVulkanDescriptorSets();
		std::vector<PushConstantRange>& GetPushConstantRanges() { return m_PushConstantRanges; }
		std::vector<VkPipelineShaderStageCreateInfo> GetPipelineShaderStageCreateInfos() { return m_ShaderStageCreateInfos; }
		const std::vector<uint32_t>& GetDynamicOffsets();

	private:
		void Reflect();
		void CreateShaderModules();
		void CreateDescriptorSetLayouts();
		void CreatePushConstantRanges();

		std::unordered_map<ShaderStage, Buffer> m_ShaderCode;

		ShaderData m_ShaderData;
		std::vector<std::vector<VulkanDescriptorSet>> m_DescriptorSets;
		std::map<uint32_t, std::unordered_map<uint32_t, Ref<UniformBuffer>>> m_UniformBuffers;

		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStageCreateInfos;
		std::vector<PushConstantRange> m_PushConstantRanges;

		std::vector<VkShaderModule> m_ShaderModules;
		std::vector<uint32_t> m_DynamicOffsets;

	};
}
#endif
