#pragma once

#include "UtilityCore.h"
#include <thread>
#include <functional>
#include <deque>
#include "UID.h"
#include "JobGraph.h"


struct JobPromise;
class Job;
class JobSystem;

//Promise handles a single graph
struct JobPromise
{
	friend class JobSystem;

public:
	JobPromise() = default;
	JobPromise(JobSystem* jobSystem, Ref<JobGraph> node);

	JobPromise(const JobPromise& copy);
	JobPromise(JobPromise&& move) noexcept;

	JobPromise& operator=(const JobPromise& copy);
	JobPromise& operator=(JobPromise&& move) noexcept;

	size_t ReturnCode() const;
	JobStatus Status() const;
	float Progress() const;

	template<typename T>
	T* Value() const;
	JobPromise Wait();
	JobPromise Then(JobGraphCallback&& callback);
	JobPromise Then(const std::string& name, JobGraphCallback&& callback);
	JobPromise Then(Ref<JobGraph> graph)
	{
		HZR_ASSERT(false, "");
		return JobPromise();
	}

protected:
	JobSystem* m_System = nullptr;
	Ref<JobGraph> m_Graph;
};

class JobSystem
{
public:
	JobSystem(uint32_t threadCount = std::thread::hardware_concurrency());
	~JobSystem();

	JobPromise SubmitGraph(Ref<JobGraph> graph);

	void WaitForJobs();
	void GetJobs();
	void SubmitJob(Ref<JobNode> node);
	size_t GetJobCount() { return m_JobCount; }

private:
	void ThreadFunc(uint32_t index);
	Ref<JobNode> GetNextAvailableJob();

	void PrintGraphDebugInfo(Ref<JobGraph> graph);

private:
	uint32_t m_ThreadCount = 0;
	std::vector<std::thread> m_Threads;
	std::atomic_size_t m_JobCount;
	std::atomic_bool m_Running = true;

	std::deque<Ref<JobNode>> m_AvailableJobNodes;
	mutable std::mutex m_JobMutex;
};

template<typename T>
inline T* JobPromise::Value() const
{
	if (!m_Graph) return nullptr;
	return m_Graph->Result<T>();
}

