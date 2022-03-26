#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Core/Pipeline/Pipeline.h"
#include "VulkanShader.h"
#include "VulkanBuffers.h"

#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(PipelineSpecification* specs);
		virtual ~VulkanPipeline();

		PipelineSpecification GetSpecifications() { return m_Specs; }
		const PipelineSpecification GetSpecifications() const { return m_Specs; }
		Ref<Shader> GetShader() { return nullptr; }
		void SetRenderPass(Ref<RenderPass> renderPass);

		void Invalidate() override;
		void Bind(Ref<RenderCommandBuffer> commandBuffer) override;
		void Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) override;
		void DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) override;

	private:
		void Destroy();

	private:
		PipelineSpecification m_Specs;

		VkPipeline m_Pipeline = nullptr;
		VkPipelineLayout m_PipelineLayout = nullptr;
		VkDescriptorSetLayout m_UniformDescriptorLayout = nullptr;

		Ref<VulkanShader> m_Shader;
	};
}
#endif
