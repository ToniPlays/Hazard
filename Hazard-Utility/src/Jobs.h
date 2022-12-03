#pragma once

#include "UtilityCore.h"
#include <thread>
#include <functional>
#include <deque>
#include "UID.h"

using JobCallback = std::function<int64_t()>;

enum JobStatus
{
	Waiting,
	Executing,
	Done,
	Error,
	Invalid
};

struct Job
{
	std::string Tag;
	UID Uid;
	JobCallback Callback;
	std::atomic<JobStatus> Status;
	std::atomic_size_t ReturnCode;
	std::atomic_size_t RefCount;

	Job() = default;
	~Job() = default;

	Job(Job&& move)
	{
		Tag = move.Tag;
		Uid = move.Uid;
		Callback = move.Callback;
		Status = move.Status.load();
		ReturnCode = move.ReturnCode.load();
		RefCount = move.RefCount.load();

		move.Tag = "";
		move.Uid = 0;
		move.Callback = nullptr;
		move.Status = JobStatus::Invalid;
		move.ReturnCode = 0;
		move.RefCount = 0;
	}
	Job& operator=(Job&& move)
	{
		Tag = move.Tag;
		Uid = move.Uid;
		Callback = move.Callback;
		Status = move.Status.load();
		ReturnCode = move.ReturnCode.load();
		RefCount = move.RefCount.load();

		move.Tag = "";
		move.Uid = 0;
		move.Callback = nullptr;
		move.Status = JobStatus::Invalid;
		move.ReturnCode = 0;
		move.RefCount = 0;

		return *this;
	}
};

class JobSystem;

struct JobPromise
{
public:
	JobPromise(JobSystem* jobSystem, UID uid);
	~JobPromise();

	JobPromise(const JobPromise& copy);
	JobPromise(JobPromise&& move);

	JobPromise& operator=(const JobPromise& copy);
	JobPromise& operator=(JobPromise&& move);

	size_t ReturnCode() const;

	JobStatus Status() const;

private:
	JobSystem* System;
	UID Uid;
};

class JobSystem
{
public:
	JobSystem(uint32_t threadCount = std::thread::hardware_concurrency());
	~JobSystem();

	JobPromise SubmitJob(JobCallback callback, const std::string& tag = "");
	void WaitForJobs();
	void GetJobs();
	Job* GetJob(UID uid);

	size_t GetJobCount() { return m_JobCount; }

	void IncJobRef(UID uid)
	{
		m_JobMutex.lock();
		Job* job = FindJob(uid);
		m_JobMutex.unlock();
		if (!job) return;

		++job->RefCount;
	}
	void DecJobRef(UID uid)
	{
		m_JobMutex.lock();
		Job* job = FindJob(uid);
		if (!job)
		{
			m_JobMutex.unlock();
			return;
		}

		if (--job->RefCount == 0)
		{
			m_Jobs.erase(m_Jobs.begin() + (job - &m_Jobs.front()));
		}
		m_JobMutex.unlock();
	}

private:
	void ThreadFunc(uint32_t index);
	Job* GetNextAvailableJob();
	Job* FindJob(UID uid);

private:
	uint32_t m_ThreadCount = 0;
	std::vector<std::thread> m_Threads;
	std::atomic_size_t m_JobCount;
	std::atomic_bool m_Running = true;

	std::deque<Job> m_Jobs;
	std::mutex m_JobMutex;
};