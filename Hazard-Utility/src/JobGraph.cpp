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
	m_Progress = copy.m_Progress.load();

	m_Buffer = copy.m_Buffer;
	m_BufferDestructor = copy.m_BufferDestructor;

	m_Dependant = copy.m_Dependant;
}

void JobGraph::AsyncJobFinished()
{
	if (--m_JobsRunning > 0)
		return;

	if (m_FinishCallback)
		m_FinishCallback(*this);

	for(auto& graph : m_DependantGraph)
		m_JobSystem->SubmitGraph(graph);

	m_Status = JobStatus::Done;
	m_Status.notify_all();
}
