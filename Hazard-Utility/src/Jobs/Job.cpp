#include "Job.h"
#include <Profiling/Timer.h>

#include "spdlog/fmt/fmt.h"

Job::~Job()
{
}

void Job::Execute()
{
	Timer timer;
	if (m_JobCallback)
	{
		try
		{
			m_JobCallback(this);
		}
		catch (JobException e)
		{
			std::cout << fmt::format("JobException: {}\n", e.what());
		}
	}

	m_ExecutionTime = timer.ElapsedMillis();
	m_Progress = 1.0f;
    
	if (m_Stage)
		m_Stage->OnJobFinished();
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
