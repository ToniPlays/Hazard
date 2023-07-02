#include "Job.h"
#include <Profiling/Timer.h>

#include "spdlog/fmt/fmt.h"

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
