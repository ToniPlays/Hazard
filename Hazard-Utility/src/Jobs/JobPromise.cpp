#include "JobPromise.h"
#include "JobGraph.h"

bool JobPromise::Succeeded() const
{
	return m_JobGraph->GetFlags() & JOB_GRAPH_SUCCEEDED;
}

void JobPromise::Wait() const
{
	if (!m_JobGraph) return;

	m_JobGraph->WaitUntilFinished();
}

bool JobPromise::HasFinished() const
{
	return m_JobGraph ? m_JobGraph->HasFinished() : true;
}

JobPromise JobPromise::Then(const std::function<void(JobGraph&)>& callback)
{
	if (!m_JobGraph) return *this;

	if (m_JobGraph->HasFinished())
		callback(*m_JobGraph);
	else 
		m_JobGraph->AddOnCompleted(callback);

	return *this;
}