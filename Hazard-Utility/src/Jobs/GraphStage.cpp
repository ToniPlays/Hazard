#include "GraphStage.h"
#include "JobGraph.h"

#include <Jobs.h>

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

	if (m_JobCount > 0) return;
	
	m_JobGraph->OnStageFinished();
}

float GraphStage::GetProgress()
{
	float result = 0;
	for (auto& job : m_Jobs)
		result += job->GetProgress() / m_Jobs.size();

	return result * m_Weight;
}