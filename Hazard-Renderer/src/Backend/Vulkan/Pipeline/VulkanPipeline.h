#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Pipeline/Pipeline.h"
#include "Backend/Core/Pipeline/RenderPass.h"

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
		void SetRenderPass(Ref<RenderPass> renderPass) override;

		void Invalidate() override;

	private:
		PipelineSpecification m_Specs;
		Ref<Shader> m_Shader;

	};
}
#endif
