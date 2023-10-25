#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Core/Rendering//Pipeline.h"
#include "Core/Rendering//RenderPass.h"

#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	class VulkanShader;

	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(PipelineSpecification* specs);
		~VulkanPipeline();

		PipelineSpecification GetSpecifications() override { return m_Specs; }
		const PipelineSpecification GetSpecifications() const override { return m_Specs; }
		Ref<Shader> GetShader() override { return m_Shader; }
		void SetRenderPass(Ref<RenderPass> renderPass) override;

		bool IsCompatibleWith(Ref<Shader> shader) const override;
		bool IsValid() const { return m_Pipeline != VK_NULL_HANDLE; }

		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

		void Invalidate() override;
		void Invalidate_RT();
		DescriptorSetLayout GetDescriptorSetLayout(uint32_t set) const;

		//Vulkan specific
		void Bind(VkCommandBuffer commandBuffer);

		VkPipelineBindPoint GetBindingPoint() const;
		VkPipeline GetVulkanPipeline() const { return m_Pipeline; }
		VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }
		VkShaderStageFlags GetPushConstantFlags() const { return m_PushConstantFlags; }

		const std::vector<VkRayTracingShaderGroupCreateInfoKHR>& GetShaderGroups() { return m_ShaderGroups; }

	private:
		void InvalidateGraphicsPipeline();
		void InvalidateComputePipeline();
		void InvalidateRaygenPipeline();
	private:
		PipelineSpecification m_Specs;
		Ref<VulkanShader> m_Shader;

		BufferLayout m_Layout;
		VkShaderStageFlags m_PushConstantFlags = 0;

		VkPipeline m_Pipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
		VkPipelineCache m_PipelineCache = VK_NULL_HANDLE;

		std::vector<VkRayTracingShaderGroupCreateInfoKHR> m_ShaderGroups;
	};
}
#endif
