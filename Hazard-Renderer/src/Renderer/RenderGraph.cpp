#include "RenderGraph.h"
#include "spdlog/fmt/fmt.h"
#include <MathCore.h>

namespace HazardRenderer
{
	void RenderGraph::Execute(Ref<RenderCommandBuffer> commandBuffer, Ref<RenderPass> outputRenderpass)
	{
		Timer timer;

		for (auto& stage : m_Stages)
		{
			RenderGraphFuncData data = {
				.CommandBuffer = commandBuffer,
				.CurrentRenderPass = outputRenderpass,
				.Enabled = stage.Enabled
			};

			stage.OnPrepare.Invoke(data);

			if (!stage.Enabled)
				continue;

			Buffer& buffer = m_Resources[stage.DebugName];
			uint64_t offset = 0;

			while (buffer.Size > offset)
			{
				data.Data = buffer.Read(offset);
				offset += stage.Stride;

				stage.Execute.Invoke(data);
			}

			stage.OnFinished.Invoke(data);
		}

		m_ExecutionTime = timer.ElapsedMillis();
	}

	void RenderGraph::SetResource(const std::string& name, void* data, uint64_t size)
	{
		m_Resources[name] = Buffer(data, size);
	}
	void RenderGraph::SetStageActive(const std::string& name, bool enabled)
	{
		for (auto& stage : m_Stages)
		{
			if (stage.DebugName != name) continue;

			stage.Enabled = enabled;
			return;
		}
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
		{
			HZR_ASSERT(createInfo->pStages[i].Stride > 0, "Stage stride must be more than 0");
			m_Stages[i] = createInfo->pStages[i];
		}
	}
}
