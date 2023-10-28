#include "RenderGraph.h"
#include "spdlog/fmt/fmt.h"
#include <MathCore.h>

namespace HazardRenderer
{
	void RenderGraph::Execute(Ref<RenderCommandBuffer> commandBuffer)
	{
		Timer timer;

		m_ExecutionTime = timer.ElapsedMillis();
	}

	Ref<RenderGraph> RenderGraph::Create(RenderGraphCreateInfo* createInfo)
	{
		return hnew RenderGraph(createInfo);
	}

	RenderGraph::RenderGraph(RenderGraphCreateInfo* createInfo) : m_DebugName(createInfo->DebugName)
	{

	}

	void RenderGraph::CreateGraphTimeline(RenderGraphCreateInfo* createInfo)
	{

	}
}
