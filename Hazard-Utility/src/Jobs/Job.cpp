#include "Job.h"
#include "JobGraph.h"
#include <Profiling/Timer.h>

#include "spdlog/fmt/fmt.h"

void Job::Execute(JobInfo info)
{
	info.ExecutionID = m_InvocationId;
	info.Current = this;
	info.Graph = m_JobGraph;

	m_Status = JobStatus::Executing;
	Timer timer;

	if (!m_JobCoroutine)
		m_JobCoroutine = m_JobCallback(info);

	try
	{
		if(!m_JobCoroutine.Done())
			m_JobCoroutine.MoveNext();

		m_ExecutionTime += timer.ElapsedMillis();

		if (m_JobCoroutine.Done())
			Finish();

		return;
	}
	catch (JobException& e)
	{
		m_Status = JobStatus::Failure;
		m_ExecutionTime = timer.ElapsedMillis();
		m_Exception = e;

		if (m_JobGraph)
			m_JobGraph->OnJobFailed(this);

		throw e;
	}
	catch (std::exception& e)
	{
		m_Status = JobStatus::Failure;
		m_ExecutionTime = timer.ElapsedMillis();

		if (m_JobGraph)
			m_JobGraph->OnJobFailed(this);
		throw e;
	}

	m_Status = JobStatus::Failure;
	std::cout << "Unknown exit " << m_JobName << std::endl;
}

void Job::Finish()
{
	if (m_Status == JobStatus::Success) return;

	m_Progress = 1.0f;

	m_Status = JobStatus::Success;
	if (m_JobGraph)
		m_JobGraph->OnJobFinished(this);
}

void Job::Progress(float progress)
{
	m_Progress = progress;
	m_Progress.notify_all();
}

void JobInfo::ContinueWith(const std::vector<Ref<Job>>& jobs)
{
	Graph->ContinueWith(jobs);
}
