#pragma once

#include "Backend/Core/RenderCommandBuffer.h"
#include "Hazard/Rendering/RendererDrawList.h"

namespace Hazard
{
	class BaseRenderer
	{
	public:
		virtual void SetCommandBuffer(Ref<HazardRenderer::RenderCommandBuffer> commandBuffer) = 0;
		virtual void Prepare(Ref<HazardRenderer::RenderPass> renderPass, const RendererDrawList& drawList) = 0;
		virtual void End() = 0;

		virtual void GeometryPass(const std::unordered_map<HazardRenderer::Pipeline*, std::vector<RawMesh>>& meshList) = 0;
		virtual void EnvironmentPass(const std::unordered_map<EnvironmentMap*, EnvironmentData>& environment) = 0;
		virtual void CompositePass(const std::unordered_map<HazardRenderer::Pipeline*, std::vector<PipelineData>>& list) = 0;
	};
}