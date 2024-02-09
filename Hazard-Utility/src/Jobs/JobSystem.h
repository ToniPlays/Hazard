#pragma once
#include <Ref.h>
#include <thread>
#include <Jobs.h>
#include "JobFlags.h"
#include "Hooks.h"
#include "Severity.h"

class JobGraph;
struct JobPromise;

class Thread : public RefCount
{
	friend class JobSystem;
public:

	Thread(uint32_t id) : m_ThreadID(id) {};
	~Thread() = default;

	uint32_t GetThreadID() const { return m_ThreadID; }
	bool IsWaiting() const { return m_Status.load() == ThreadStatus::Waiting; }
	ThreadStatus GetStatus() const { return m_Status; }
	Ref<Job> GetCurrentJob() const { return m_CurrentJob; }

	void Join() { m_Thread.join(); };
	void Detach() { m_Thread.detach(); };
	void WaitForIdle() { m_Status.wait(ThreadStatus::Executing); }

private:

	void Execute(Ref<Job> job);
private:

	std::thread m_Thread;
	uint32_t m_ThreadID = 0;
	Ref<Job> m_CurrentJob = nullptr;
	std::atomic<ThreadStatus> m_Status = ThreadStatus::Waiting;
};

class JobSystem
{
	friend class Thread;
	friend class JobGraph;

public:
	enum JobSystemHook
	{
		Submit = 0,
		Status,
		Finished,
		Failure,
		Message
	};

public:
	JobSystem(uint32_t threads = std::thread::hardware_concurrency());
	~JobSystem();

	const std::vector<Ref<Thread>>& GetThreads() const { return m_Threads; }
	std::vector<Ref<JobGraph>> GetQueuedGraphs() const { return m_QueuedGraphs; }

	void WaitForJobsToFinish();
	void Terminate();

	JobPromise QueueGraph(Ref<JobGraph> graph);
	uint64_t WaitForUpdate();

	void Hook(JobSystemHook hook, const std::function<void(Ref<JobGraph>)>& callback)
	{
		m_Hooks.AddHook(hook, callback);
	}
	void Hook(JobSystemHook hook, const std::function<void(Ref<Thread>, ThreadStatus)>& callback)
	{
		m_StatusHook.Add(callback);
	}
	void Hook(JobSystemHook hook, const std::function<void(Severity, const std::string&)>& callback)
	{
		m_MessageHook.Add(callback);
	}

private:
	bool QueueJobs(const std::vector<Ref<Job>>& jobs);
	void TerminateGraphJobs(Ref<JobGraph> graph);
	void OnGraphFinished(Ref<JobGraph> graph);

	Ref<Job> FindAvailableJob();
	void ThreadFunc(Ref<Thread> thread);

private:
	std::vector<Ref<JobGraph>> m_QueuedGraphs;

	std::vector<Ref<Thread>> m_Threads;
	std::vector<Ref<Job>> m_Jobs;
	std::atomic_bool m_Running = false;

	std::atomic_uint64_t m_JobCount = 0;
	std::atomic_uint64_t m_RunningJobCount = 0;

	std::mutex m_JobMutex;
	std::mutex m_RunningJobMutex;
	std::mutex m_GraphMutex;
	
	Hooks<JobSystemHook, void(Ref<JobGraph>)> m_Hooks;
	Callback<void(Severity, const std::string&)> m_MessageHook;
	Callback<void(Ref<Thread>, ThreadStatus)> m_StatusHook;
};
