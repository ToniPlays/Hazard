#include "Jobs.h"

#include "spdlog/fmt/fmt.h"

size_t JobPromise::ReturnCode() const
{
	if (!System) return JobStatus::Invalid;
	Job* job = System->GetJob(Uid);
	if (!job) return 0;
	return job->ReturnCode;
};

JobStatus JobPromise::Status() const
{
	if (!System) return JobStatus::Invalid;
	Job* job = System->GetJob(Uid);
	if (!job) return JobStatus::Invalid;
	return job->Status;
};
float JobPromise::Progress() const
{
	if (!System) return JobStatus::Invalid;
	Job* job = System->GetJob(Uid);
	if (!job) return JobStatus::Invalid;
	return job->Progress;
};

JobPromise JobPromise::Wait()
{
	if (!System) return *this;
	Job* job = System->GetJob(Uid);
	if (!job) return *this;

	//Wait until status is Done or Error
	job->Status.wait(JobStatus::Waiting);
	job->Status.wait(JobStatus::Executing);
	return *this;
}

JobPromise JobPromise::Then(JobCallback&& callback)
{
	if (!System)
		return JobPromise(nullptr, 0);

	Job* dependency = System->GetJob(Uid);
	return System->SubmitJob(dependency->Tag, callback, dependency->Uid);
}

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

JobPromise JobSystem::SubmitJob(const std::string& tag, JobCallback callback, UID dependency)
{
	UID id = UID();
	Job job = {};
	job.Tag = tag;
	job.Uid = id;
	job.Dependency = dependency;
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
	try
	{
		m_JobMutex.lock();
		Job* job = FindJob(uid);
		m_JobMutex.unlock();
		return job;
	}
	catch (const std::exception& e)
	{
		m_JobMutex.unlock();
		HZR_ASSERT(false, e.what());
	}
	return nullptr;
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
		job->Status.notify_all();
		m_JobMutex.unlock();

		job->ReturnCode = job->Callback(this, job);
		job->Progress = 1.0f;

		if (job->ReturnCode >= 0)
			job->Status = JobStatus::Done;
		else
			job->Status = JobStatus::Error;
		job->Status.notify_all();

		m_JobCount--;
		m_JobCount.notify_all();
	}
}

Job* JobSystem::GetNextAvailableJob()
{
	for (auto& job : m_Jobs)
	{
		if (job.Status == JobStatus::Waiting)
		{
			if (job.Dependency == 0)
				return &job;

			Job* dependency = FindJob(job.Dependency);
			if (!dependency) return &job;

			if (dependency->Status >= JobStatus::Done)
				return &job;
		}
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
