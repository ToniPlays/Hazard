#pragma once

#include "JobSystem.h"
#include "JobFlags.h"

class Job;
class JobGraph;

class JobPromise
{
	friend class JobSystem;

public:
	JobPromise() = default;
	JobPromise(Ref<Job> job) : m_Job(job) {};
	JobPromise(Ref<JobGraph> graph) : m_JobGraph(graph) {};

	bool Succeeded() { return m_JobGraph->GetFlags() & JOB_FLAGS_SUCCEEDED; }

	void Wait()
	{
		if (m_JobGraph)
			m_JobGraph->Wait();
	}

	Buffer GetResult() 
	{
		return m_JobGraph->GetResult();
	}

	std::vector<Buffer> GetResults()
	{
		return m_JobGraph->GetResults();
	}

private:

	Ref<JobGraph> m_JobGraph = nullptr;
	Ref<Job> m_Job = nullptr;
};
