#include "UtilityCore.h"
#include "Jobs.h"

#include "spdlog/fmt/fmt.h"

size_t JobPromise::ReturnCode() const
{
	return 0;
};

JobStatus JobPromise::Status() const
{
	if (!m_Graph) return Invalid;
	return m_Graph->m_Status;
};
float JobPromise::Progress() const
{
	if (!m_Graph) return 1.0f;
	return m_Graph->m_Progress;
};

JobPromise JobPromise::Wait()
{
	if (!m_Graph) return *this;

	m_Graph->m_Status.wait(JobStatus::Waiting);
	m_Graph->m_Status.wait(JobStatus::Executing);

	return *this;
}
JobPromise JobPromise::Then(JobGraphCallback&& callback)
{
	std::string name = m_Graph->Name() + " Then()";
	return Then(name, std::move(callback));
}

JobPromise JobPromise::Then(const std::string& name, JobGraphCallback&& callback)
{
	if (!m_System)
		return JobPromise();

	Ref<JobGraph> graph = Ref<JobGraph>::Create(name);

	graph->m_DependencyGraphs.push_back(m_Graph);
	graph->m_DebugName = name;
	graph->m_Status = JobStatus::Waiting;
	graph->OnFinished(callback);

	m_Graph->m_DependantGraph.push_back(graph);

	JobPromise promise = {};
	promise.m_System = m_System;
	promise.m_Graph = graph;

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
	if (!graph)
		return JobPromise();

	graph->m_JobSystem = this;
	m_JobMutex.lock();

	for (auto& job : graph->Jobs())
		m_AvailableJobNodes.push_back(job);

	m_JobMutex.unlock();
#if HZR_DEBUG && false
	if (!graph->Name().empty())
		PrintGraphDebugInfo(graph);
#endif
	m_JobCount = m_AvailableJobNodes.size();
	m_JobCount.notify_all();

	JobPromise promise = {};
	promise.m_System = this;
	promise.m_Graph = graph;

	if (graph->Jobs().size() == 0)
	{
		graph->m_Status = JobStatus::Executing;
		graph->m_Status.notify_all();
		graph->AsyncJobFinished(nullptr);
	}

	return promise;
}

void JobSystem::WaitForJobs()
{

}

void JobSystem::GetJobs()
{

}

void JobSystem::SubmitJob(Ref<JobNode> node)
{
	m_JobMutex.lock();
	m_AvailableJobNodes.push_back(node);
	m_JobMutex.unlock();
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

		if (job->m_JobGraph)
		{
			job->m_JobGraph->m_Status = JobStatus::Executing;
			job->m_JobGraph->m_Status.notify_all();
		}

		job->JobStarted();

		if (job->Callback)
			job->m_ReturnCode = job->Callback(*job.Raw());

		job->JobFinished();
		
		m_JobMutex.lock();
		{
			if (--job->m_RemainingDependencies == 0 && job->m_Dependant.size() > 0)
				m_AvailableJobNodes.insert(m_AvailableJobNodes.begin(), job->m_Dependant.begin(), job->m_Dependant.end());

			m_JobCount = m_AvailableJobNodes.size();
		}
		m_JobMutex.unlock();
		m_JobCount.notify_all();
	}
}

Ref<JobNode> JobSystem::GetNextAvailableJob()
{
	Ref<JobNode> node = m_AvailableJobNodes.front();
	m_AvailableJobNodes.pop_front();
	return node;
}

void JobSystem::PrintGraphDebugInfo(Ref<JobGraph> graph)
{
	std::cout << fmt::format("\nSubmit: Job Graph: \"{0}\", ({1})\n - Jobs: ({2})", graph->m_DebugName, graph->m_Status, graph->Jobs().size()) << std::endl;

	for (auto& job : graph->Jobs())
		std::cout << fmt::format("   - Job: \"{0}\"", job->DebugName) << std::endl;

	std::cout << " - Dependency graphs:" << std::endl;

	for (auto& dependency : graph->m_DependencyGraphs)
		std::cout << fmt::format("   - Dependency: \"{0}\" ({1})", dependency->m_DebugName, dependency->m_Status) << std::endl;

	std::cout << " - Dependant graphs:" << std::endl;
	for (auto& dependant : graph->m_DependantGraph)
		std::cout << fmt::format("   - Dependant: \"{0}\"", dependant->m_DebugName) << std::endl;
}

JobPromise::JobPromise(JobSystem* jobSystem, Ref<JobGraph> node)
	: m_System(jobSystem), m_Graph(node) {}

JobPromise::JobPromise(const JobPromise& copy)
	: m_System(copy.m_System), m_Graph(copy.m_Graph)
{

}

JobPromise::JobPromise(JobPromise&& move) noexcept
	: m_System(move.m_System), m_Graph(move.m_Graph)
{

}

JobPromise& JobPromise::operator=(const JobPromise& copy)
{
	m_System = copy.m_System;
	m_Graph = copy.m_Graph;
	return *this;
}

JobPromise& JobPromise::operator=(JobPromise&& move) noexcept
{
	m_System = move.m_System;
	m_Graph = move.m_Graph;
	move.m_System = nullptr;
	move.m_Graph = nullptr;

	return *this;
}
