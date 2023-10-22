#include "GraphStage.h"
#include "JobGraph.h"

#include <Jobs.h>

GraphStage::~GraphStage()
{

}
Ref<GraphStage> GraphStage::GetGraphStage(uint32_t index)
{
	if (!m_JobGraph)
		return nullptr;

	return m_JobGraph->GetStage(index);
}

void GraphStage::QueueJobs(const std::vector<Ref<Job>>& jobs)
{
	m_JobMutex.lock();
	m_Jobs.insert(m_Jobs.begin(), jobs.begin(), jobs.end());

	for (uint32_t i = 0; i < jobs.size(); i++)
	{
		Ref<Job> job = jobs[i];
		job->m_InvocationId = i;
		job->m_Stage = this;
	}

	m_JobMutex.unlock();

	m_JobCount = m_Jobs.size();
	m_JobCount.notify_all();
}

Ref<JobGraph> GraphStage::GetGraph()
{
	return m_JobGraph;
}

Buffer GraphStage::GetJobResult(uint32_t index)
{
	return m_Jobs[index]->GetResult();
}

std::vector<Buffer> GraphStage::GetJobResults()
{
	std::vector<Buffer> results;
	results.reserve(m_Jobs.size());

	for (auto& job : m_Jobs)
		results.push_back(job->GetResult());

	return results;
}

void GraphStage::OnJobFinished(Ref<Job> job)
{
	m_JobCount--;
	m_JobCount.notify_all();

	if (job->GetStatus() == JobStatus::Failure && !(m_Flags & JOB_FLAGS_SILENT_FAILURE))
		m_Flags &= ~JOB_FLAGS_SUCCEEDED;

	if (m_JobCount == 0)
		m_JobGraph->OnStageFinished(this);
}

float GraphStage::GetProgress()
{
	float result = 0;
	for (auto& job : m_Jobs)
		result += job->GetProgress() / m_Jobs.size();

	return result * m_Weight;
}
