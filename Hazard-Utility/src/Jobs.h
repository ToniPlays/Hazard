#pragma once

#include "UtilityCore.h"
#include <thread>
#include <functional>
#include <deque>
#include "UID.h"

#define JOB_PROGRESS(job, x, y) job->Progress = (float)x / (float)y;

struct JobPromise;
struct Job;
class JobSystem;

using JobCallback = std::function<int64_t(JobSystem* system, Job* job)>;
using JobDataDestructor = void(*)(void*);

enum JobStatus
{
	Invalid = 0,
	Waiting,
	Executing,
	Done,
	Error
};

struct Job
{
	std::string Tag;
	UID Uid;
	JobCallback Callback;
	std::atomic<JobStatus> Status;
	std::atomic_size_t ReturnCode;
	std::atomic_size_t RefCount;
	std::atomic<float> Progress;
	void* Buffer;
	JobDataDestructor BufferDestructor;

	UID Dependency = 0;

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
		Buffer = move.Buffer;
		BufferDestructor = move.BufferDestructor;
		Dependency = move.Dependency;

		move.Tag = "";
		move.Uid = 0;
		move.Status = JobStatus::Invalid;
		move.ReturnCode = 0;
		move.RefCount = 0;
		move.Buffer = nullptr;
		move.BufferDestructor = nullptr;
		move.Dependency = 0;
	}
	Job& operator=(Job&& move) noexcept
	{
		Tag = move.Tag;
		Uid = move.Uid;
		Callback = move.Callback;
		Status = move.Status.load();
		ReturnCode = move.ReturnCode.load();
		RefCount = move.RefCount.load();
		Buffer = move.Buffer;
		BufferDestructor = move.BufferDestructor;
		Dependency = Dependency;

		move.Tag = "";
		move.Uid = 0;
		move.Status = JobStatus::Invalid;
		move.ReturnCode = 0;
		move.RefCount = 0;
		move.Buffer = nullptr;
		move.BufferDestructor = nullptr;
		move.Dependency = 0;

		return *this;
	}

	template<typename T>
	T* Value()
	{
		return reinterpret_cast<T*>(Buffer);
	}
};

template<typename T>
struct TypedJobPromise;

struct JobPromise
{
	friend class JobSystem;
public:
	JobPromise() = default;
	JobPromise(JobSystem* jobSystem, UID uid);
	~JobPromise();

	JobPromise(const JobPromise& copy);
	JobPromise(JobPromise&& move);

	JobPromise& operator=(const JobPromise& copy);
	JobPromise& operator=(JobPromise&& move);

	size_t ReturnCode() const;
	JobStatus Status() const;
	float Progress() const;
	JobPromise Wait();

	JobPromise Then(JobCallback&& callback);

	template<typename T>
	TypedJobPromise<T> TypedThen(JobCallback&& callback);

protected:
	JobSystem* System = nullptr;
	UID Uid = 0;
};

template<typename T>
struct TypedJobPromise : public JobPromise
{
	T* Value() const;
};


class JobSystem
{
public:
	JobSystem(uint32_t threadCount = std::thread::hardware_concurrency());
	~JobSystem();

	JobPromise SubmitJob(const std::string& tag, JobCallback callback, UID dependency = 0);
	template<typename T, typename... Args>
	TypedJobPromise<T> SubmitTypedJob(const std::string& tag, JobCallback callback, Args&&... args, UID dependency = 0)
	{
		UID id = UID();
		Job job = {};
		job.Tag = tag;
		job.Uid = id;
		job.Dependency = dependency;
		job.Status = JobStatus::Waiting;
		job.Callback = callback;
		job.ReturnCode = 0;

		job.Buffer = new T(std::forward<Args>(args)...);
		job.BufferDestructor = [](void* ptr) { delete reinterpret_cast<T*>(ptr); };

		m_JobMutex.lock();
		m_Jobs.emplace_back(std::move(job));
		m_JobMutex.unlock();

		++m_JobCount;
		m_JobCount.notify_one();

		TypedJobPromise<T> promise = {};
		promise.System = this;
		promise.Uid = id;

		return promise;
	}
	void WaitForJobs();
	void GetJobs();
	Job* GetJob(UID uid);

	size_t GetJobCount() { return m_JobCount; }

	void IncJobRef(UID uid)
	{
		Job* job = GetJob(uid);
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
			for (size_t i = 0; i < m_Jobs.size(); i++)
			{
				Job& j = m_Jobs[i];
				if (j.Uid != job->Uid) 
					continue;

				//m_Jobs.erase(m_Jobs.begin() + i);
				//std::cout << "Removed job" << std::endl;
				break;
			}
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
	mutable std::mutex m_JobMutex;
};

template<typename T>
inline TypedJobPromise<T> JobPromise::TypedThen(JobCallback&& callback)
{
	if (!System)
		return TypedJobPromise<T>();

	Job* dependency = System->GetJob(Uid);
	return System->SubmitTypedJob<T>(dependency->Tag, callback, dependency->Uid);
}

template<typename T>
inline T* TypedJobPromise<T>::Value() const
{
	if (!System) return nullptr;
	Job* job = System->GetJob(Uid);
	if (!job) return nullptr;

	return reinterpret_cast<T*>(job->Buffer);
}

