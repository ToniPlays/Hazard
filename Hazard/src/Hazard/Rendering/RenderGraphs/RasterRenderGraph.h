#pragma once

#include "Renderer/RenderGraph.h"

namespace Hazard
{
	static Ref<HazardRenderer::RenderGraph> CreateRasterGraph() 
	{
		using namespace HazardRenderer;

		RenderGraphCreateInfo info = {};
		info.DebugName = "RasterRenderGraph";
		info.StageCount = 0;

		return RenderGraph::Create(&info);
	}
}