#include "RenderGraph.h"
#include "spdlog/fmt/fmt.h"
#include <MathCore.h>

namespace HazardRenderer
{
	void RenderGraph::Execute(void* userData)
	{
		Timer timer;

		RenderGraphFuncData data = {};
		data.m_UserData = userData;
		data.Iteration = m_Iterations;
		data.Enabled = true;

		m_OnPrepare.Invoke(data);

		for (auto& stage : m_Stages)
		{
			data.Enabled = stage.Enabled;
			stage.OnPrepare.Invoke(data);

			if (!stage.Enabled)
				continue;

			Buffer& buffer = m_Resources[stage.DebugName];
			uint64_t offset = 0;

			while (buffer.Size > offset)
			{
				data.m_DrawData = buffer.Read(offset);
				offset += stage.Stride;

				stage.Execute.Invoke(data);
			}

			stage.OnFinished.Invoke(data);
		}

		m_OnFinished.Invoke(data);

		m_Iterations++;
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

		m_OnPrepare = createInfo->OnPrepare;
		m_OnFinished = createInfo->OnFinished;

		for (uint32_t i = 0; i < createInfo->StageCount; i++)
		{
			HZR_ASSERT(createInfo->pStages[i].Stride > 0, "Stage stride must be more than 0");
			m_Stages[i] = createInfo->pStages[i];
		}
	}
}
