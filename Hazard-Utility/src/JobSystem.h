#pragma once
#include <Ref.h>
#include <thread>
#include <Jobs.h>
#include "GraphStage.h"


enum class ThreadStatus
{
	None,
	Terminated,
	Waiting,
	Executing
};

class JobGraph;

class Thread : public RefCount
{
	friend class JobSystem;
public:

	Thread(uint32_t threadID);
	~Thread();

	uint32_t GetThreadID() const { return m_ThreadID; }
	bool IsWaiting() const { return m_Status.load() == ThreadStatus::Waiting; }
	ThreadStatus GetStatus() const { return m_Status; }
	Ref<Job> GetCurrentJob() const { return m_CurrentJob; }

	void Join() { m_Thread.join(); };
	void Detach() { m_Thread.detach(); };
	void WaitForIdle() { m_Status.wait(ThreadStatus::Executing); }

private:
	std::thread m_Thread;
	uint32_t m_ThreadID = 0;
	Ref<Job> m_CurrentJob;
	std::atomic<ThreadStatus> m_Status = ThreadStatus::None;
};

class JobSystem
{
public:
	JobSystem(uint32_t threads = std::thread::hardware_concurrency());
	~JobSystem();

	const std::vector<Ref<Thread>>& GetThreads() { return m_Threads; }

	void QueueJob(Ref<Job> job);
	void QueueJobs(const std::vector<Ref<Job>>& jobs);

	void QueueGraph(Ref<JobGraph> graph);

	uint32_t WaitForUpdate()
	{
		if (m_JobCount != 0)
			m_JobCount.wait(m_JobCount);
		if (m_RunningJobs != 0)
			m_RunningJobs.wait(m_RunningJobs);

		return m_JobCount;
	}

	void WaitForJobsToFinish();
	void Terminate();

private:

	Ref<Job> FindAvailableJob();

	void ThreadFunc(Ref<Thread> thread);

private:
	std::vector<Ref<Thread>> m_Threads;
	std::vector<Ref<Job>> m_Jobs;
	std::atomic_bool m_Running = false;

	std::atomic_uint32_t m_JobCount = 0;
	std::atomic_uint32_t m_RunningJobs = 0;

	std::mutex m_JobMutex;
};