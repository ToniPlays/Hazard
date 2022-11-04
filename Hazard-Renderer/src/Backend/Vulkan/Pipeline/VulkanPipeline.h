#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Pipeline/Pipeline.h"
#include "Backend/Core/Pipeline/RenderPass.h"

#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	class VulkanShader;

	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(PipelineSpecification* specs);
		~VulkanPipeline();

		PipelineSpecification GetSpecifications() { return m_Specs; }
		const PipelineSpecification GetSpecifications() const { return m_Specs; }
		Ref<Shader> GetShader() { return m_Shader; }
		void SetRenderPass(Ref<RenderPass> renderPass) override;

		bool IsCompatibleWith(Ref<Shader> shader) const;
		bool IsValid() const { return m_Pipeline != VK_NULL_HANDLE; }

		void SetLayout(const BufferLayout& layout) { m_Layout = layout; };

		void Invalidate() override;
		void Invalidate_RT();

		//Vulkan specific
		void Bind(VkCommandBuffer commandBuffer);

		VkPipelineBindPoint GetBindingPoint() const;
		VkPipeline GetVulkanPipeline() { return m_Pipeline; }
		VkPipelineLayout GetPipelineLayout() { return m_PipelineLayout; }

	private:
		void InvalidateGraphicsPipeline();
		void InvalidateComputePipeline();
	private:
		PipelineSpecification m_Specs;
		Ref<VulkanShader> m_Shader;

		BufferLayout m_Layout;

		VkPipeline m_Pipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
		VkPipelineCache m_PipelineCache = VK_NULL_HANDLE;
	};
}
#endif
