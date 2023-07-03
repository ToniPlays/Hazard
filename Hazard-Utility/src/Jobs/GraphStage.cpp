#include "GraphStage.h"
#include "JobGraph.h"

#include <Jobs.h>

GraphStage::~GraphStage()
{
	m_ResultBuffer.Release();
}
Ref<GraphStage> GraphStage::GetGraphStage(uint32_t index)
{
	if (!m_JobGraph)
		return nullptr;

	return m_JobGraph->GetStage(index);
}

void GraphStage::QueueJobs(const std::vector<Ref<Job>>& jobs)
{
	std::lock_guard lock(m_JobMutex);
	m_Jobs.insert(m_Jobs.begin(), jobs.begin(), jobs.end());

	m_JobCount = m_Jobs.size();
	m_JobCount.notify_all();

	for (auto& job : m_Jobs)
		job->m_Stage = this;
}

void GraphStage::OnJobFinished()
{
	m_JobCount--;
	m_JobCount.notify_all();

	if (m_JobCount == 0)
		m_JobGraph->OnStageFinished();
}

float GraphStage::GetProgress()
{
	float result = 0;
	for (auto& job : m_Jobs)
		result += job->GetProgress() / m_Jobs.size();

	return result * m_Weight;
}
