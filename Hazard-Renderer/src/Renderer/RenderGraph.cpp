#include "RenderGraph.h"
#include "spdlog/fmt/fmt.h"
#include <MathCore.h>

namespace HazardRenderer
{
	void RenderGraph::Execute(Ref<RenderCommandBuffer> commandBuffer)
	{
		Timer timer;

		for (auto& stage : m_Stages)
		{
			if (!stage.Enabled)
			{
				stage.OnDisabled(commandBuffer);
				continue;
			}

			stage.OnPrepare(commandBuffer);

			Buffer buffer = m_Resources[stage.DebugName];

			uint64_t offset = 0;
			while (buffer.Size > offset)
			{
				stage.Execute(commandBuffer, buffer.Read(stage.DataSize, offset));
				offset += stage.DataSize;
			}

			stage.OnFinished(commandBuffer);
		}

		m_ExecutionTime = timer.ElapsedMillis();
	}

	void RenderGraph::SetResource(const std::string& name, void* data, uint64_t size)
	{
		m_Resources[name] = Buffer(data, size);
	}

	Ref<RenderGraph> RenderGraph::Create(RenderGraphCreateInfo* createInfo)
	{
		return hnew RenderGraph(createInfo);
	}

	RenderGraph::RenderGraph(RenderGraphCreateInfo* createInfo) : m_DebugName(createInfo->DebugName)
	{
		m_DebugName = createInfo->DebugName;
		m_Stages.resize(createInfo->StageCount);

		for (uint32_t i = 0; i < createInfo->StageCount; i++)
			m_Stages[i] = createInfo->pStages[i];
	}

	void RenderGraph::CreateGraphTimeline(RenderGraphCreateInfo* createInfo)
	{

	}
}
