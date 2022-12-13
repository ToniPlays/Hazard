#include "JobGraph.h"
#include "Jobs.h"

#include "spdlog/fmt/fmt.h"

JobNode::JobNode(const JobNode& copy)
{
	DebugName = copy.DebugName;
	Callback = copy.Callback;
	Weight = copy.Weight;

	m_RemainingDependencies = copy.m_RemainingDependencies.load();
	m_Status = copy.m_Status.load();
	m_ReturnCode = copy.m_ReturnCode.load();
	m_RefCount = copy.m_RefCount.load();

	m_Buffer = copy.m_Buffer;
	m_BufferDestructor = copy.m_BufferDestructor;

	m_Dependant = copy.m_Dependant;
}

void JobNode::JobFinished()
{
	m_Status = JobStatus::Done;
	m_Status.notify_all();

	for (auto& dependant : m_Dependant)
	{
		if (--dependant->m_RemainingDependencies == 0)
			m_JobGraph->GetSystem().SubmitJob(dependant);
	}

	m_JobGraph->AsyncJobFinished(this);
}

void JobNode::SetProgress(float progress)
{
	m_Progress = progress * Weight;
	m_JobGraph->UpdateProgress();
}
void JobGraph::AsyncJobFinished(Ref<JobNode> node)
{
	if (m_JobsRunning == 0) {}
	else if (--m_JobsRunning > 0)
		return;

	if (m_FinishCallback)
		m_FinishCallback(*this);

	m_Status = JobStatus::Done;
	m_Status.notify_all();
	m_Progress = 1.0f;

	for(auto& graph : m_DependantGraph)
		m_JobSystem->SubmitGraph(graph);

}

void JobGraph::PushNode(Ref<JobNode> node)
{
	node->m_JobGraph = this;
	node->m_Status = JobStatus::Waiting;
	m_AllJobs.push_back(node);
	m_JobsRunning++;

	if (node->m_RemainingDependencies == 0 && m_JobSystem)
		m_JobSystem->SubmitJob(node);
}
void JobGraph::Then(Ref<JobNode> node)
{
	for (auto& job : m_AllJobs)
		job->m_Dependant.push_back(node);

	node->m_JobGraph = this;
	node->m_Status = JobStatus::Waiting;
	node->m_Dependencies = m_AllJobs;
	node->m_RemainingDependencies = m_AllJobs.size();

	m_JobsRunning++;
	m_AllJobs.push_back(node);
}

void JobGraph::UpdateProgress()
{
	float totalProgress = 0.0f;

	for (auto& job : m_AllJobs)
		totalProgress += job->m_Progress;

	m_Progress = totalProgress;
	HZR_ASSERT(m_Progress <= 1.0f, "");
}
