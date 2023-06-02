#include "JobGraph.h"

#include "GraphStage.h"
#include "JobSystem.h"

JobGraph::JobGraph(const std::string& name, uint32_t count) : m_Name(name)
{
	m_Stages.resize(count);
	for (uint32_t i = 0; i < count; i++)
	{
		m_Stages[i] = Ref<GraphStage>::Create(i, 1.0f / (float)count);
		m_Stages[i]->m_JobGraph = this;
	}
}
Ref<GraphStage> JobGraph::GetNextStage()
{
	if (m_CurrentStage + 1 == m_Stages.size())
		return nullptr;

	return m_Stages[m_CurrentStage + 1];
}

Ref<JobGraph> JobGraph::Execute()
{
	for (auto& stage : m_Stages)
	{
		for (auto& job : stage->m_Jobs)
			job->Execute();
	}
	return this;
}

float JobGraph::GetProgress()
{
	float progress = 0;
	for (auto& stage : m_Stages)
	{
		if (stage->m_JobCount == 0)
			progress += stage->GetWeight();
		else
			progress += stage->GetProgress();
	}

	return progress;
}

void JobGraph::OnStageFinished()
{
	Ref<GraphStage> next = GetNextStage();
	m_CurrentStage++;
	m_CurrentStage.notify_all();

	if (!next)
		return;

	if (next->GetJobs().size() == 0)
		OnStageFinished();

	if (m_JobSystem)
		m_JobSystem->QueueJobs(next->m_Jobs);
}
