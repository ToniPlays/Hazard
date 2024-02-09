#include "JobSystem.h"
#include "JobGraph.h"

#include <spdlog/fmt/fmt.h>
#include <Profiling/Timer.h>

void Thread::Execute(Ref<Job> job)
{
	if (job == nullptr) return;

	m_CurrentJob = job;

	JobInfo info = {};
	info.Thread = this;

	m_Status = ThreadStatus::Executing;
	try
	{
		job->Execute(info);
		m_Status = ThreadStatus::Finished;
	}
	catch (JobException e)
	{
		m_Status = ThreadStatus::Failed;
		throw e;
	}
}

JobSystem::JobSystem(uint32_t threads) : m_Running(true)
{
	m_Threads.resize(threads);

	for (uint32_t i = 0; i < threads; i++)
	{
		m_Threads[i] = Ref<Thread>::Create(i);
		m_Threads[i]->m_Thread = std::thread(&JobSystem::ThreadFunc, this, m_Threads[i]);
	}
}
JobSystem::~JobSystem()
{
	m_Running = false;
	for (auto& thread : m_Threads)
		thread->Join();
}

void JobSystem::ThreadFunc(Ref<Thread> thread)
{
	//Initialize thread state
	while (m_Running)
	{
		thread->m_Status = ThreadStatus::Waiting;
		thread->m_Status.notify_all();

		//Wait if no jobs
		m_JobCount.wait(0);
		if (!m_Running) break;

		m_JobMutex.lock();
		Ref<Job> job = FindAvailableJob();

		if (!job)
		{
			m_JobMutex.unlock();
			continue;
		}

		m_RunningJobCount++;
		{
			auto it = std::find(m_Jobs.begin(), m_Jobs.end(), job);

			if (it != m_Jobs.end())
				m_Jobs.erase(it);
		}

		m_JobMutex.unlock();
		m_RunningJobCount.notify_all();

		m_JobCount = m_Jobs.size();
		m_JobCount.notify_one();

		try
		{
			//Execute job
			thread->m_CurrentJob = job;

			m_StatusHook.Invoke(thread, ThreadStatus::Executing);
			thread->Execute(job);
			m_StatusHook.Invoke(thread, thread->GetStatus());
		}
		catch (JobException e)
		{
			std::string msg = fmt::format("JobException: {0} {1}", job->GetName(), e.what());
			m_MessageHook.Invoke(Severity::Error, msg);
			m_StatusHook.Invoke(thread, thread->GetStatus());
		}

		thread->m_CurrentJob = nullptr;

		m_RunningJobCount--;
		m_RunningJobCount.notify_all();
	}

	thread->m_Status = ThreadStatus::Terminated;
	thread->m_Status.notify_all();

	std::string msg = fmt::format("Thread {} terminated", thread->GetThreadID());
	m_MessageHook.Invoke(Severity::Info, msg);
}

bool JobSystem::QueueJobs(const std::vector<Ref<Job>>& jobs)
{
	if (jobs.size() == 0) return false;

	m_JobMutex.lock();
	m_Jobs.insert(m_Jobs.end(), jobs.begin(), jobs.end());
	m_JobCount = m_Jobs.size();
	m_JobMutex.unlock();

	m_JobCount.notify_all();

	std::string msg = fmt::format("Queuing {} jobs", jobs.size());
	m_MessageHook.Invoke(Severity::Info, msg);
	return true;
}

void JobSystem::TerminateGraphJobs(Ref<JobGraph> graph)
{
	m_JobMutex.lock();
	std::vector<Ref<Job>> jobs;
	jobs.reserve(m_JobCount);

	for (auto& job : m_Jobs)
	{
		if (job->m_JobGraph != graph)
			jobs.emplace_back(job);
	}

	m_Jobs = jobs;
	m_JobMutex.unlock();
}

void JobSystem::OnGraphFinished(Ref<JobGraph> graph)
{
	m_GraphMutex.lock();
	auto it = std::find(m_QueuedGraphs.begin(), m_QueuedGraphs.end(), graph);
	if (it != m_QueuedGraphs.end())
		m_QueuedGraphs.erase(it);

	m_GraphMutex.unlock();
}

void JobSystem::WaitForJobsToFinish()
{
	while (m_JobCount != 0)
		m_JobCount.wait(m_JobCount);

	for (auto& thread : m_Threads)
		thread->WaitForIdle();
}
void JobSystem::Terminate()
{
	m_Running = false;
	m_JobCount = 1;
	m_JobCount.notify_all();
}

JobPromise JobSystem::QueueGraph(Ref<JobGraph> graph)
{
	if (!graph)
	{
		std::string msg = "Tried submitting null graph, aborting";
		m_MessageHook.Invoke(Severity::Warning, msg);
		return JobPromise();
	}

	m_GraphMutex.lock();
	m_QueuedGraphs.push_back(graph);
	m_GraphMutex.unlock();

	graph->Execute(this);

	m_Hooks.Invoke(JobSystemHook::Submit, graph);
	return JobPromise(graph);
}

uint64_t JobSystem::WaitForUpdate()
{
	if (m_JobCount != 0)
		m_JobCount.wait(m_JobCount);
	if (m_RunningJobCount != 0)
		m_RunningJobCount.wait(m_RunningJobCount);

	return m_JobCount;
}

Ref<Job> JobSystem::FindAvailableJob()
{
	for (auto& job : m_Jobs)
		return job;

	return nullptr;
}

