#pragma once

#include "JobGraph.h"

class Job;

template<typename T>
class JobPromise
{
	friend class JobSystem;
public:

	JobPromise() = default;
	JobPromise(Ref<Job> job) : m_Job(job) {};
	JobPromise(Ref<JobGraph> graph) : m_JobGraph(graph) {};
	JobPromise(T result) 
	{
		if constexpr (IsRef<T>::value)
		{
			result->IncRefCount();
		}
		m_Result.Release();
		m_Result.Allocate(sizeof(T));
		m_Result.Write(&result, sizeof(T));
	};

	void Wait()
	{
		//if (m_JobGraph)
		//	m_JobGraph->Wait();
	}
	T Result()
	{
		//if (m_JobGraph)
		//	return m_JobGraph->GetResult<T>();
		if (m_Result.Data)
			return m_Result.Read<T>();
		return T();
	}
private:

	void* GetResultBuffer() { return nullptr;  };

private:
	Ref<JobGraph> m_JobGraph = nullptr;
	Ref<Job> m_Job = nullptr;
	Buffer m_Result;
};
