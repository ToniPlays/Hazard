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

    bool Succeeded();
    void Wait();

    Buffer GetResult();
    std::vector<Buffer> GetResults();

private:

	Ref<JobGraph> m_JobGraph = nullptr;
	Ref<Job> m_Job = nullptr;
};
