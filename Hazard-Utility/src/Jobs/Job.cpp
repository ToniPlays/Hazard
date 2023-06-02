#include "Job.h"
#include <Profiling/Timer.h>

void Job::Execute()
{
	Timer timer;
	if (m_JobCallback)
		m_JobCallback(this);
	m_ExecutionTime = timer.ElapsedMillis();
	m_Progress = 1.0f;
    
	if (m_Stage)
		m_Stage->OnJobFinished();
}
