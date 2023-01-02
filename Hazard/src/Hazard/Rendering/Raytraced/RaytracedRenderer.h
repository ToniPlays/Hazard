#pragma once

#include "../BaseRenderer.h"

namespace Hazard
{
	class RaytracedRenderer : public BaseRenderer
	{
	public:
		void SetCommandBuffer(Ref<HazardRenderer::RenderCommandBuffer> commandBuffer) override
		{
			m_CommandBuffer = commandBuffer;
		}
		void Prepare(Ref<HazardRenderer::RenderPass> renderPass, const RendererDrawList& drawList) override;
		void End() override {};

		void GeometryPass(const MeshDrawList& drawList) override;
		void EnvironmentPass(const std::unordered_map<EnvironmentMap*, EnvironmentData>& environment) override;
		void CompositePass(const std::unordered_map<HazardRenderer::Pipeline*, std::vector<PipelineData>>& list) override;

	private:
		Ref<HazardRenderer::RenderCommandBuffer> m_CommandBuffer;
		Ref<HazardRenderer::RenderPass> m_RenderPass;
	};
}
