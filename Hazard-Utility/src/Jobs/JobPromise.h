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
	JobPromise(Ref<JobGraph> graph) : m_JobGraph(graph) {};

    bool Succeeded() const;
    void Wait() const;
	bool Valid() const { return m_JobGraph != nullptr; }
	JobPromise Then(const std::function<void(JobGraph&)>& callback);
	const std::string& GetJobName() const { return m_JobGraph->GetName(); }

	template<typename T>
	T GetResult() const 
	{
		if (!m_JobGraph) return T();
		return m_JobGraph->GetResult<T>();
	}
	template<typename T>
	std::vector<T> GetResults() const
	{
		if (!m_JobGraph) return std::vector<T>();
		return m_JobGraph->GetResults<T>();
	}

private:
	Ref<JobGraph> m_JobGraph = nullptr;
};
