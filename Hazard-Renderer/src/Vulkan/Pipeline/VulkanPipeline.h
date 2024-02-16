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

		const PipelineSpecification& GetSpecifications() const override { return m_Specs; }
		Ref<Shader> GetShader() override { return m_Shader; }
		void SetRenderPass(Ref<RenderPass> renderPass) override;

		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

		void Invalidate() override;
		void Invalidate_RT();

		//Vulkan specific
		void Bind(VkCommandBuffer commandBuffer);

		VkPipelineBindPoint GetBindingPoint() const;
		VkPipeline GetVulkanPipeline() const { return m_Pipelines.at(m_CurrentRenderpass); }
		VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }

		const std::vector<VkRayTracingShaderGroupCreateInfoKHR>& GetShaderGroups() { return m_ShaderGroups; }

	private:
		void InvalidateGraphicsPipeline();
		void InvalidateComputePipeline();
		void InvalidateRaygenPipeline();

		void Release();

	private:
		PipelineSpecification m_Specs;
		Ref<VulkanShader> m_Shader;

		BufferLayout m_Layout;
		VkRenderPass m_CurrentRenderpass = VK_NULL_HANDLE;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
		VkPipelineCache m_PipelineCache = VK_NULL_HANDLE;

		std::vector<VkRayTracingShaderGroupCreateInfoKHR> m_ShaderGroups;
		std::unordered_map<VkRenderPass, VkPipeline> m_Pipelines;
	};
}
#endif
