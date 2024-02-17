#include "EnvironmentRenderer.h"

namespace Hazard
{
	void EnvironmentRenderer::Init(Ref<HazardRenderer::RenderCommandBuffer> commandBuffer)
	{
		m_CommandBuffer = commandBuffer;
	}
	void EnvironmentRenderer::Prepare(EnvironmentData& environment, Ref<RenderPass> renderPass)
	{
		m_Environment = &environment;
		if (m_Environment->Pipeline)
			m_Environment->Pipeline->SetRenderPass(renderPass);
	}

	void EnvironmentRenderer::Render(Ref<HazardRenderer::DescriptorSet> descriptor)
	{
		EnvironmentData& data = *m_Environment;
		if (!data.Pipeline) return;

		m_CommandBuffer->SetPipeline(data.Pipeline);
		m_CommandBuffer->SetDescriptorSet(descriptor, 0);
		m_CommandBuffer->PushConstants(Buffer(&data.Constants, sizeof(EnvironmentData::SkyboxConstants)), 0, SHADER_STAGE_FRAGMENT_BIT);
		m_CommandBuffer->Draw(6);
	}
}
