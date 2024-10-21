#pragma once

#include "JobFlags.h"
#include "JobGraph.h"
#include "Ref.h"

class Job;
class JobSystem;

template<typename T>
class Promise
{
	friend class JobSystem;

public:
	Promise() = default;
    
    const std::string& GetJobName() const
    {
        return m_JobGraph->GetName();
    }
    
    bool Succeeded() const 
    {
        return m_JobGraph->GetFlags() & JOB_GRAPH_SUCCEEDED;
    }
    
    Promise<T> Wait() const
    {
        if(m_JobGraph)
            m_JobGraph->Wait();
        return *this;
    }
    
    Promise<T> ContinueWith(const std::function<void(std::vector<T>)>&) { return *this; }
    Promise<T> Catch(std::function<void(const JobException&)> callback) {
        if(m_JobGraph)
            m_JobGraph->AddOnFailed(callback);
        return *this;
    }
private:
    Promise(Ref<JobGraph> graph) : m_JobGraph(graph) {}
private:
	Ref<JobGraph> m_JobGraph = nullptr;
};
