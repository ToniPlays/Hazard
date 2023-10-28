#include "JobPromise.h"
#include "JobGraph.h"

bool JobPromise::Succeeded()
{
    if (!m_JobGraph) return false;

    return m_JobGraph->GetFlags() & JOB_FLAGS_SUCCEEDED;
}

void JobPromise::Wait()
{
    if (m_JobGraph)
        m_JobGraph->Wait();
}
