#include "Job.h"
#include "JobGraph.h"
#include <Profiling/Timer.h>

#include "spdlog/fmt/fmt.h"

Job::~Job() 
{
	m_DestroyQueue.Invoke();
	m_DestroyQueue.Clear();
}

void Job::Execute(JobInfo& info)
{
	info.Job = this;
	info.ParentGraph = m_JobGraph;
	info.ExecutionID = m_InvocationId;

	m_Status = JobStatus::Executing;
	Timer timer;

	if (!m_JobCallback)
	{
		m_Progress = 1.0f;
		m_Status = JobStatus::Success;
		m_JobGraph->OnJobFinished(this);
		return;
	}

	try
	{
		m_JobCallback(info);
		m_Status = JobStatus::Success;
		m_Progress = 1.0f;
		m_JobGraph->OnJobFinished(this);
	}
	catch (JobException e)
	{
		m_Status = JobStatus::Failure;
		m_ExecutionTime = timer.ElapsedMillis();

		m_JobGraph->OnJobFailed(this);
		throw e;
	}

	m_ExecutionTime = timer.ElapsedMillis();
	m_Progress = 1.0f;
}

void Job::Progress(float progress)
{
	m_Progress = progress;
	m_Progress.notify_all();
}
