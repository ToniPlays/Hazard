#include "JobSystem.h"
#include "GraphStage.h"
#include "JobGraph.h"

#include <spdlog/fmt/fmt.h>
#include <Profiling/Timer.h>


JobSystem::JobSystem(uint32_t threads) : m_Running(true)
{
	m_Threads.resize(threads);
	
	m_LaunchThread = std::this_thread::get_id();

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
	thread->m_IsMainThread = thread->m_Thread.get_id() == m_LaunchThread;
	if (thread->IsMainThread())
	{
		thread->m_Status == ThreadStatus::Terminated;
		return;
	}

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
			m_RunningJobCount++;
			m_RunningJobCount.notify_all();
			{
				auto it = std::find(m_Jobs.begin(), m_Jobs.end(), job);

				if (it != m_Jobs.end())
					m_Jobs.erase(it);
			}

			m_JobMutex.unlock();

			m_JobCount = m_Jobs.size();
			m_JobCount.notify_one();

			thread->Execute(job, this);

			m_RunningJobCount--;
			m_RunningJobCount.notify_all();
		}
		else m_JobMutex.unlock();
	}

	thread->m_Status = ThreadStatus::Terminated;
	thread->m_Status.notify_all();
}

JobPromise JobSystem::QueueJob(Ref<Job> job)
{
	m_JobMutex.lock();
	m_Jobs.emplace_back(job);
	m_JobCount = m_Jobs.size();
	m_JobMutex.unlock();

	m_JobCount.notify_one();

	return JobPromise(job);
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

JobPromise JobSystem::QueueGraph(Ref<JobGraph> graph)
{
	{
		std::scoped_lock lock(m_GraphMutex);
		m_QueuedGraphs.push_back(graph);
	}

	QueueGraphJobs(graph);
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
void JobSystem::QueueGraphJobs(Ref<JobGraph> graph)
{
	if (!graph) return;

	graph->m_JobSystem = this;
	Ref<GraphStage> stage = graph->GetStage(0);
	HZR_ASSERT(stage->m_JobCount > 0, "Stage has no jobs");
	QueueJobs(stage->m_Jobs);
}

Ref<Job> JobSystem::FindAvailableJob()
{
	for (auto& job : m_Jobs)
		return job;

	return nullptr;
}

void Thread::Execute(Ref<Job> job, JobSystem* system)
{
	m_CurrentJob = job;
	if (job == nullptr) return;

	m_Status = ThreadStatus::Executing;
	m_Status.notify_all();

	JobInfo info = {};
	info.Thread = this;
	if (job->GetJobGraph())
	{
		info.PreviousStage = job->GetJobGraph()->GetPreviousStage();
		info.NextStage = job->GetJobGraph()->GetNextStage();
		info.ParentGraph = job->GetJobGraph();
	}

	job->Execute(info);

	if (job->GetJobGraph())
	{
		//Check if this was the last job to run on graph
		if (job->GetJobGraph()->HasFinished())
		{
			std::scoped_lock lock(system->m_GraphMutex);
			auto it = std::find(system->m_QueuedGraphs.begin(), system->m_QueuedGraphs.end(), job->GetJobGraph());
			if (it != system->m_QueuedGraphs.end())
				system->m_QueuedGraphs.erase(it);
		}
	}

	m_CurrentJob = nullptr;
	m_Status = ThreadStatus::Waiting;
	m_Status.notify_all();
}
