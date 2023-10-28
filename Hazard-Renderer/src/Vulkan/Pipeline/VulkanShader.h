#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include <map>

#include "Core/Rendering/Shader.h"
#include "Core/Rendering/GPUBuffer.h"
#include "Core/Rendering/Texture.h"
#include "VulkanDescriptorSet.h"
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	class VulkanImage2D;

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const std::unordered_map<uint32_t, std::string>& shaderModules);
		~VulkanShader();

		void Reload() override;
		void SetShaderCode(const std::unordered_map<uint32_t, Buffer>& shaderCode) override
		{
			m_ShaderCode = shaderCode;
		};

		std::unordered_map<uint32_t, Buffer> GetShaderCode() const override { return m_ShaderCode; };
		

		//Vulkan specific
		const std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStageCreateInfos() const { return m_ShaderStageCreateInfos;  }
		void Reload_RT(bool forceCompile = false);

	private:
		void CreateShaderModules();

		std::unordered_map<uint32_t, Buffer> m_ShaderCode;

		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStageCreateInfos;
		std::unordered_map<std::string, PushConstantRange> m_PushConstantRanges;

		std::vector<VkShaderModule> m_ShaderModules;
		std::vector<uint32_t> m_DynamicOffsets;
	};
}
#endif
