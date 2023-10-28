#include "JobGraph.h"

#include "GraphStage.h"
#include "JobSystem.h"

JobGraph::JobGraph(const std::string& name, uint32_t count, uint32_t flags) : m_Name(name)
{
	m_Flags |= flags;

	m_Stages.resize(count);
	for (uint32_t i = 0; i < count; i++)
	{
		m_Stages[i] = Ref<GraphStage>::Create(i, 1.0f / (float)count, flags);
		m_Stages[i]->m_JobGraph = this;
	}
}
Ref<GraphStage> JobGraph::GetNextStage()
{
	if (m_CurrentStage + 1 >= m_Stages.size())
		return nullptr;

	return m_Stages[m_CurrentStage + 1];
}
Ref<GraphStage> JobGraph::GetPreviousStage()
{
	if ((int64_t)m_CurrentStage - 1 < 0)
		return nullptr;

	return m_Stages[m_CurrentStage - 1];
}

Ref<GraphStage> JobGraph::AddStage()
{
	Ref<GraphStage> stage = Ref<GraphStage>::Create(m_Stages.size(), 0.0f, m_Flags);
	stage->m_JobGraph = this;
	m_Stages.push_back(stage);
	return stage;
}

void JobGraph::CombineStages(Ref<JobGraph> graph, uint32_t offset)
{
	if (graph == nullptr) return;

	auto& stages = graph->GetStages();
	for (uint32_t i = 0; i < stages.size(); i++)
	{
		Ref<GraphStage> stage = GetStage(i + offset);
		if (!stage)
			stage = AddStage();

		stage->m_JobGraph = this;
		stage->QueueJobs(stages[i]->m_Jobs);
	}
}

Ref<JobGraph> JobGraph::Execute()
{
	JobInfo info = {};
	for (auto& stage : m_Stages)
	{
		for (auto& job : stage->m_Jobs)
			job->Execute(info);
	}
	return this;
}

float JobGraph::GetProgress()
{
	float progress = 0;
	for (auto& stage : m_Stages)
		progress += stage->GetProgress();

	return progress;
}
void JobGraph::OnStageFinished(Ref<GraphStage> stage)
{
	Ref<GraphStage> next = GetNextStage();
	m_CurrentStage++;
	m_CurrentStage.notify_all();

	if (!next)
		return;

	if (next->GetJobs().size() == 0)
		OnStageFinished(nullptr);

	if (m_JobSystem)
		m_JobSystem->QueueJobs(next->m_Jobs);
}
