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
	if (!m_Graph) return *this;

	m_Graph->m_Status.wait(JobStatus::Waiting);
	m_Graph->m_Status.wait(JobStatus::Executing);

	return *this;
}
JobPromise JobPromise::Then(JobGraphCallback&& callback, const std::string& name)
{
	if (!m_System)
		return JobPromise();

	Ref<JobNode> node = Ref<JobNode>::Create();
	node->m_Status = JobStatus::Waiting;
	node->m_Buffer = m_Graph->Result<void*>();
	node->m_RemainingDependencies = 1;

	Ref<JobGraph> graph = Ref<JobGraph>::Create();

	graph->m_DependencyGraphs.push_back(m_Graph);
	graph->m_DebugName = name.empty() ? m_Graph->Name() + " Dependant" : name;
	graph->m_Status = JobStatus::Waiting;
	graph->OnFinished(callback);
	graph->AsyncJob(node);

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
	if (graph->Jobs().size() == 0)
		return JobPromise();

	graph->m_JobSystem = this;

	Ref<JobNode> rootNode = Ref<JobNode>::Create();
	rootNode->m_Status = JobStatus::Waiting;
	rootNode->DebugName = fmt::format("{} (Root)", graph->Name());
	rootNode->m_RemainingDependencies = graph->Jobs().size();
	rootNode->m_Dependencies.reserve(graph->Jobs().size());

	m_JobMutex.lock();
	for (auto& job : graph->Jobs())
	{
		rootNode->m_Dependencies.push_back(job);
		job->m_Dependant.push_back(rootNode);
		m_AvailableJobNodes.push_back(job);
	}

	m_JobMutex.unlock();
	m_JobCount = m_AvailableJobNodes.size();
	m_JobCount.notify_all();

	JobPromise promise = {};
	promise.m_System = this;
	promise.m_Graph = graph;
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
		m_JobCount.wait(m_JobCount);
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

		job->m_Status = JobStatus::Executing;
		job->m_Status.notify_all();

		if (job->Callback)
			job->Callback(*job.Raw());

		job->m_Progress = 1.0f;

		job->m_Status = JobStatus::Done;
		job->m_Status.notify_all();

		if (--job->m_RemainingDependencies == 0 && job->m_Dependant.size() > 0)
		{
			m_JobMutex.lock();
			m_AvailableJobNodes.insert(m_AvailableJobNodes.begin(), job->m_Dependant.begin(), job->m_Dependant.end());
			m_JobMutex.unlock();

			if (job->m_JobGraph)
				job->m_JobGraph->AsyncJobFinished();
		}

		m_JobMutex.lock();
		m_JobCount = m_AvailableJobNodes.size();
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

JobPromise::JobPromise(JobSystem* jobSystem, Ref<JobGraph> node)
	: m_System(jobSystem), m_Graph(node) {}

JobPromise::~JobPromise()
{

}

JobPromise::JobPromise(const JobPromise& copy) : m_System(copy.m_System), m_Graph(copy.m_Graph)
{

}

JobPromise::JobPromise(JobPromise&& move)
	: m_System(move.m_System), m_Graph(move.m_Graph)
{

}

JobPromise& JobPromise::operator=(const JobPromise& copy)
{
	m_System = copy.m_System;
	m_Graph = copy.m_Graph;
	return *this;
}

JobPromise& JobPromise::operator=(JobPromise&& move)
{
	m_System = move.m_System;
	m_Graph = move.m_Graph;
	move.m_System = nullptr;
	move.m_Graph = nullptr;

	return *this;
}
