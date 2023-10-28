#include "Job.h"
#include "JobGraph.h"
#include <Profiling/Timer.h>

#include "spdlog/fmt/fmt.h"

Job::~Job()
{
	if (m_Result)
		hdelete m_Result;
}
Ref<JobGraph> Job::GetJobGraph()
{
	return m_Stage->GetGraph();
}

void Job::Execute(JobInfo& info)
{
	info.Job = this;
	info.ExecutionID = m_InvocationId;

	m_Status = JobStatus::Executing;
	Timer timer;

	if (m_JobCallback)
	{
		try
		{
			m_JobCallback(info);
			m_Status = JobStatus::Success;
		}
		catch (JobException e)
		{
			std::cout << fmt::format("JobException: {}\n", e.what());
			m_Status = JobStatus::Failure;
		}
	}

	m_ExecutionTime = timer.ElapsedMillis();
	m_Progress = 1.0f;

	if (m_Stage)
		m_Stage->OnJobFinished(this);
}

float Job::WaitForUpdate()
{
	m_Progress.wait(m_Progress);
	return m_Progress;
}

void Job::Progress(float progress)
{
	m_Progress = progress;
	m_Progress.notify_all();
}
