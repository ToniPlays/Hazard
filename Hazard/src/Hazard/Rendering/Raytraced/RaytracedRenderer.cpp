#include "hzrpch.h"
#include "RaytracedRenderer.h"


namespace Hazard 
{
	void RaytracedRenderer::Prepare(Ref<HazardRenderer::RenderPass> renderPass, const RendererDrawList& drawList)
	{

	}
	void RaytracedRenderer::GeometryPass(const std::unordered_map<HazardRenderer::Pipeline*, std::vector<RawMesh>>& meshList)
	{

	}
	void RaytracedRenderer::EnvironmentPass(const std::unordered_map<EnvironmentMap*, EnvironmentData>& environment)
	{

	}
	void RaytracedRenderer::CompositePass(const std::unordered_map<HazardRenderer::Pipeline*, std::vector<PipelineData>>& list)
	{

	}
}