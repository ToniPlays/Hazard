#include "Jobs.h"

size_t JobPromise::ReturnCode() const
{
	if (!System) return SIZE_MAX;
	return System->GetJob(Uid)->ReturnCode;
};

JobStatus JobPromise::Status() const
{
	if (!System) return JobStatus::Invalid;
	return System->GetJob(Uid)->Status;
};


JobSystem::JobSystem(uint32_t threadCount) : m_ThreadCount(threadCount)
{
	m_Threads.reserve(threadCount);
	for (uint32_t i = 0; i < threadCount; i++)
		m_Threads.emplace_back(&JobSystem::ThreadFunc, this, i);
}

JobSystem::~JobSystem()
{
	m_Running = false;
	for (auto& thread : m_Threads)
		thread.join();
}

JobPromise JobSystem::SubmitJob(JobCallback callback, const std::string& tag)
{
	UID id = UID();
	Job job = {};
	job.Tag = tag;
	job.Uid = id;
	job.Status = JobStatus::Waiting;
	job.Callback = callback;
	job.ReturnCode = 0;

	m_JobMutex.lock();
	m_Jobs.emplace_back(std::move(job));
	m_JobMutex.unlock();

	++m_JobCount;
	m_JobCount.notify_one();
	return JobPromise(this, id);
}

void JobSystem::WaitForJobs()
{
	
}

void JobSystem::GetJobs()
{

}

Job* JobSystem::GetJob(UID uid)
{
	m_JobMutex.lock();
	Job* job = FindJob(uid);
	m_JobMutex.unlock();
	return job;
}

Job* JobSystem::FindJob(UID uid)
{
	for (auto& job : m_Jobs)
		if (job.Uid == uid)
			return &job;

	return nullptr;
}

void JobSystem::ThreadFunc(uint32_t index)
{
	while (m_Running)
	{
		m_JobCount.wait(m_JobCount);
		if (!m_Running)
			break;

		m_JobMutex.lock();
		Job* job = GetNextAvailableJob();
		
		if (!job)
		{
			m_JobMutex.unlock();
			continue;
		}
		job->Status = JobStatus::Executing;
		m_JobMutex.unlock();

		job->ReturnCode = job->Callback();

		if (job->ReturnCode == 0)
			job->Status = JobStatus::Done;
		else 
			job->Status = JobStatus::Error;

	}
}

Job* JobSystem::GetNextAvailableJob()
{
	for (auto& job : m_Jobs)
	{
		if (job.Status == JobStatus::Waiting)
			return &job;
	}
	return nullptr;
}

JobPromise::JobPromise(JobSystem* jobSystem, UID uid)
	: System(jobSystem), Uid(uid)
{
	if (System)
		System->IncJobRef(uid);
}

JobPromise::~JobPromise()
{
	if (System)
		System->DecJobRef(Uid);
}

JobPromise::JobPromise(const JobPromise& copy) : System(copy.System), Uid(copy.Uid)
{
	if (System)
		System->IncJobRef(Uid);
}

JobPromise::JobPromise(JobPromise&& move)
	: System(move.System), Uid(move.Uid)
{
	move.System = nullptr;
	move.Uid = 0;
}

JobPromise& JobPromise::operator=(const JobPromise& copy)
{
	System = copy.System;
	Uid = copy.Uid;
	if (System)
		System->IncJobRef(Uid);
	return *this;
}

JobPromise& JobPromise::operator=(JobPromise&& move)
{
	System = move.System;
	Uid = move.Uid;
	move.System = nullptr;
	move.Uid = 0;

	return *this;
}
