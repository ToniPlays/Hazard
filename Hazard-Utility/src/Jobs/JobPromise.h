#pragma once

#include "JobSystem.h"
#include "JobFlags.h"

class Job;
class JobGraph;

struct JobPromise
{
	friend class JobSystem;

public:
	JobPromise() = default;
	JobPromise(Ref<Job> job) : m_Job(job) {};
	JobPromise(Ref<JobGraph> graph) : m_JobGraph(graph) {};

    bool Succeeded();
    void Wait();
	const std::string& GetJobName() const { return m_JobGraph->GetName(); }

	template<typename T>
	std::vector<T> GetResults()
	{
		if (!m_JobGraph) return std::vector<T>();
		return m_JobGraph->GetResults<T>();
	}

private:
	Ref<JobGraph> m_JobGraph = nullptr;
	Ref<Job> m_Job = nullptr;
};
