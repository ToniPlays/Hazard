#pragma once

#include "Hazard/Rendering/Pipeline/Pipeline.h"
#include "VulkanShader.h"
#include "VulkanBuffers.h"

#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan
{
	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(const PipelineSpecification& specs);
		virtual ~VulkanPipeline();

		virtual PipelineSpecification GetSpecifications() { return m_Specs; }
		virtual const PipelineSpecification GetSpecifications() const { return m_Specs; }
		virtual Ref<VertexBuffer> GetBuffer() override { return m_VertexBuffer; }
		virtual Ref<Shader> GetShader() { return m_Shader; }

		virtual void Invalidate() override;
		virtual void Bind(Ref<RenderCommandBuffer> commandBuffer) override;
		virtual void Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) override;

	private:
		PipelineSpecification m_Specs;

		VkPipeline m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
		VkDescriptorSetLayout m_UniformDescriptorLayout;

		Ref<VulkanVertexBuffer> m_VertexBuffer;
		Ref<VulkanIndexBuffer> m_IndexBuffer;
		Ref<VulkanShader> m_Shader;
	};
}