#include "UtilityCore.h"
#include "Jobs.h"

#include "spdlog/fmt/fmt.h"

size_t JobPromise::ReturnCode() const
{
	return 0;
};

JobStatus JobPromise::Status() const
{
	return JobStatus::Invalid;
};
float JobPromise::Progress() const
{
	return 0;
};

JobPromise JobPromise::Wait()
{
	if (!m_Node) return *this;

	m_Node->m_Status.wait(JobStatus::Waiting);
	m_Node->m_Status.wait(JobStatus::Executing);

	return *this;
}

JobPromise JobPromise::Then(JobCallback&& callback)
{
	if (!m_System)
		return JobPromise();

	Ref<JobNode> node = Ref<JobNode>::Create();
	node->m_Status = JobStatus::Waiting;
	node->DebugName = m_Node->DebugName;
	node->m_RemainingDependencies = 1;
	node->m_Uid = UID();
	node->Callback = callback;

	m_Node->m_Dependant = node;

	JobPromise promise = {};
	promise.m_System = m_System;
	promise.m_Node = node;

	return promise;
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

JobPromise JobSystem::SubmitGraph(Ref<JobGraph> graph)
{
	if (graph->Jobs().size() == 0)
		return JobPromise();

	Ref<JobNode> rootNode = Ref<JobNode>::Create();
	rootNode->m_Status = JobStatus::Waiting;
	rootNode->DebugName = fmt::format("{} (Root)", graph->Name());
	rootNode->m_RemainingDependencies = graph->Jobs().size();
	rootNode->m_Uid = UID();

	m_JobMutex.lock();
	for (auto& job : graph->Jobs())
	{
		job->m_Dependant = rootNode;
		m_AvailableJobNodes.push_back(job);
	}


	m_JobMutex.unlock();
	m_JobCount = m_AvailableJobNodes.size();
	m_JobCount.notify_all();

	std::cout << fmt::format("Submitted job for graph {} (Jobs: {})", rootNode->DebugName, m_JobCount) << std::endl;

	JobPromise promise = {};
	promise.m_System = this;
	promise.m_Node = rootNode;
	return promise;
}

void JobSystem::WaitForJobs()
{

}

void JobSystem::GetJobs()
{

}

void JobSystem::ThreadFunc(uint32_t index)
{
	while (m_Running)
	{
		m_JobCount.wait(0);
		if (!m_Running)
			break;

		m_JobMutex.lock();
		if (m_AvailableJobNodes.size() == 0)
		{
			m_JobMutex.unlock();
			continue;
		}

		Ref<JobNode> job = GetNextAvailableJob();
		m_JobMutex.unlock();

		job->m_System = this;
		job->m_Status = JobStatus::Executing;
		job->m_Status.notify_all();

		std::cout << fmt::format("Started job {0}", job->DebugName) << std::endl;
		if (job->Callback)
			job->Callback(*job.Raw());

		std::cout << fmt::format("Finished job {0}", job->DebugName) << std::endl;
		job->m_Progress = 1.0f;

		if (job->m_ReturnCode >= 0)
			job->m_Status = JobStatus::Done;
		else
			job->m_Status = JobStatus::Error;
		job->m_Status.notify_all();


		if (--job->m_RemainingDependencies == 0 && job->m_Dependant)
		{
			m_JobMutex.lock();
			m_AvailableJobNodes.push_back(job->m_Dependant);
			job->m_Dependant->m_Buffer = job->m_Buffer;
			job->m_BufferDestructor = job->m_BufferDestructor;
			std::cout << fmt::format("Submitted dependant {} for job {}", job->m_Dependant->DebugName, job->DebugName) << std::endl;
			m_JobMutex.unlock();
		}

		m_JobCount = m_AvailableJobNodes.size();
		m_JobCount.notify_all();
	}
}

Ref<JobNode> JobSystem::GetNextAvailableJob()
{
	Ref<JobNode> node = m_AvailableJobNodes.front();
	m_AvailableJobNodes.pop_front();
	return node;
}

JobPromise::JobPromise(JobSystem* jobSystem, Ref<JobNode> node)
	: m_System(jobSystem), m_Node(node)
{
}

JobPromise::~JobPromise()
{

}

JobPromise::JobPromise(const JobPromise& copy) : m_System(copy.m_System), m_Node(copy.m_Node)
{
}

JobPromise::JobPromise(JobPromise&& move)
	: m_System(move.m_System), m_Node(move.m_Node)
{
}

JobPromise& JobPromise::operator=(const JobPromise& copy)
{
	m_System = copy.m_System;
	m_Node = copy.m_Node;
	return *this;
}

JobPromise& JobPromise::operator=(JobPromise&& move)
{
	m_System = move.m_System;
	m_Node = move.m_Node;
	move.m_System = nullptr;
	move.m_Node = nullptr;

	return *this;
}
