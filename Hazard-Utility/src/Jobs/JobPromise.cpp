#include "JobPromise.h"
#include "JobGraph.h"

bool JobPromise::Succeeded()
{
    return m_JobGraph->GetFlags() & JOB_FLAGS_SUCCEEDED;
}

void JobPromise::Wait()
{
    if (m_JobGraph)
        m_JobGraph->Wait();
}
Buffer JobPromise::GetResult()
{
    return m_JobGraph->GetResult();
}

std::vector<Buffer> JobPromise::GetResults()
{
    return m_JobGraph->GetResults();
}
