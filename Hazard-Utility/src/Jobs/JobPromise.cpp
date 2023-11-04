#include "JobPromise.h"
#include "JobGraph.h"

bool JobPromise::Succeeded() const
{
    if (!m_JobGraph) return false;

    return m_JobGraph->GetFlags() & JOB_FLAGS_SUCCEEDED;
}

void JobPromise::Wait() const
{
    if (m_JobGraph)
        m_JobGraph->Wait();
}
void JobPromise::Then(const std::function<void()> callback)
{
    if (m_JobGraph)
        m_JobGraph->AddOnFinished(callback);
}
