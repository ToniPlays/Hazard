#include "JobSystem.h"
#include "GraphStage.h"
#include "JobGraph.h"

#include <spdlog/fmt/fmt.h>
#include <Profiling/Timer.h>

Thread::Thread(uint32_t threadID) : m_ThreadID(threadID)
{
}

Thread::~Thread()
{

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

		if (job)
		{
			m_RunningJobs++;
			m_RunningJobs.notify_all();

			thread->m_Status = ThreadStatus::Executing;
			thread->m_Status.notify_all();
			{
				auto it = std::find(m_Jobs.begin(), m_Jobs.end(), job);

				if (it != m_Jobs.end())
					m_Jobs.erase(it);

			}
			m_JobCount = m_Jobs.size();
			m_JobCount.notify_one();
			m_JobMutex.unlock();

			thread->m_CurrentJob = job;
			{
				job->Execute();
			}
			thread->m_CurrentJob = nullptr;
			m_RunningJobs--;
			m_RunningJobs.notify_all();
		}
		else m_JobMutex.unlock();
	}

	thread->m_Status = ThreadStatus::Terminated;
	thread->m_Status.notify_all();
}

JobPromise<bool> JobSystem::QueueJob(Ref<Job> job)
{
	m_JobMutex.lock();
	m_Jobs.emplace_back(job);
	m_JobCount = m_Jobs.size();
	m_JobMutex.unlock();

	m_JobCount.notify_one();

	return JobPromise<bool>(job);
}
void JobSystem::QueueJobs(const std::vector<Ref<Job>>& jobs)
{
	if (jobs.size() == 0) return;

	m_JobMutex.lock();
	m_Jobs.insert(m_Jobs.end(), jobs.begin(), jobs.end());
	m_JobCount = m_Jobs.size();
	m_JobMutex.unlock();

	m_JobCount.notify_all();
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

void JobSystem::QueueGraphJobs(Ref<JobGraph> graph)
{
	if (!graph) return;

	graph->m_JobSystem = this;
	Ref<GraphStage> stage = graph->GetStage(0);
	QueueJobs(stage->m_Jobs);
}

Ref<Job> JobSystem::FindAvailableJob()
{
	for (auto& job : m_Jobs)
	{
		return job;
	}
	return nullptr;
}
