#include "Job.h"
#include "JobGraph.h"
#include <Profiling/Timer.h>

#include "spdlog/fmt/fmt.h"

void JobInfo::ContinueWith(Ref<Job> job)
{
    Graph->ContinueWith({ job });
}


void Job::Execute(JobInfo& info)
{
	info.Current = this;
	info.Graph = m_JobGraph;
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
		m_ExecutionTime = timer.ElapsedMillis();

		m_JobGraph->OnJobFinished(this);
	}
	catch (JobException e)
	{
		m_Status = JobStatus::Failure;
		m_ExecutionTime = timer.ElapsedMillis();
        m_Exception = e;
		m_JobGraph->OnJobFailed(this);
		throw e;
	}

	m_Progress = 1.0f;
}

void Job::Progress(float progress)
{
	m_Progress = progress;
	m_Progress.notify_all();
}
