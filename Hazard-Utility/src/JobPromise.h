#pragma once

#include "JobGraph.h"

class Job;

template<typename T>
class JobPromise
{
	friend class JobSystem;
public:

	JobPromise() = default;

	void Wait()
	{
		/*if (m_JobGraph)
			m_JobGraph->Wait();
			*/
	}
	T Get()
	{
		return T();
		/*if (m_JobGraph)
			return m_JobGraph->GetResult<T>();
			*/
	}
private:
	JobPromise(Ref<Job> job) : m_Job(job) {};
	JobPromise(Ref<JobGraph> graph) : m_JobGraph(graph) {};

	void* GetResultBuffer() { return nullptr;  };

private:
	Ref<JobGraph> m_JobGraph = nullptr;
	Ref<Job> m_Job = nullptr;
};