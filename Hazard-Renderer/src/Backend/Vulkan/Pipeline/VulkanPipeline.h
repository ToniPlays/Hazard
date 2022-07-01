#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Pipeline/Pipeline.h"
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
		Ref<Shader> GetShader() { return m_Shader; }
		void SetRenderPass(Ref<RenderPass> renderPass);

		void Invalidate() override;
		void Bind(Ref<RenderCommandBuffer> commandBuffer) override;
		void Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) override;
		void DrawInstanced(Ref<RenderCommandBuffer> commandBuffer, uint32_t count, uint32_t instanceCount) override;
		void DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) override;

	private:
		void Destroy();
		void RT_Invalidate();

	private:
		PipelineSpecification m_Specs;

		VkPipeline m_Pipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
		VkDescriptorSetLayout m_UniformDescriptorLayout = VK_NULL_HANDLE;

		Ref<VulkanShader> m_Shader;
	};
}
#endif
