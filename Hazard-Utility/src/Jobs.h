#pragma once

#include "UtilityCore.h"
#include <thread>
#include <functional>
#include <deque>
#include "UID.h"
#include "JobGraph.h"


struct JobPromise;
struct Job;
class JobSystem;


struct JobPromise
{
	friend class JobSystem;
public:
	JobPromise() = default;
	JobPromise(JobSystem* jobSystem, Ref<JobNode> node);
	~JobPromise();

	JobPromise(const JobPromise& copy);
	JobPromise(JobPromise&& move);

	JobPromise& operator=(const JobPromise& copy);
	JobPromise& operator=(JobPromise&& move);

	size_t ReturnCode() const;
	JobStatus Status() const;
	float Progress() const;

	template<typename T>
	T* Value() const;
	JobPromise Wait();

	JobPromise Then(JobCallback&& callback);

protected:
	JobSystem* m_System = nullptr;
	Ref<JobNode> m_Node;

};

class JobSystem
{
public:
	JobSystem(uint32_t threadCount = std::thread::hardware_concurrency());
	~JobSystem();

	JobPromise SubmitGraph(Ref<JobGraph> graph);
	void WaitForJobs();
	void GetJobs();

	size_t GetJobCount() { return m_JobCount; }

	void IncJobRef(UID uid)
	{

	}
	void DecJobRef(UID uid)
	{

	}

private:
	void ThreadFunc(uint32_t index);
	Ref<JobNode> GetNextAvailableJob();

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
	return nullptr;
	if (!m_System) return nullptr;
	return reinterpret_cast<T*>(nullptr);
}

