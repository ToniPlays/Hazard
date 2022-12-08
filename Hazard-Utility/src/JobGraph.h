#pragma once

#include "UtilityCore.h"
#include "UID.h"
#include "Ref.h"

#include <functional>
#include <atomic>

class JobSystem;
class Job;

using JobDataDestructor = void(*)(void*);

enum JobStatus
{
	Invalid = 0,
	Waiting,
	Executing,
	Done,
	Error
};

struct JobGraph;

struct JobNode : public RefCount
{
	friend struct JobGraph;
	friend class JobSystem;
	friend struct JobPromise;

	JobNode() = default;
	JobNode(const JobNode& other);

	std::string DebugName;
	std::function<int(JobNode&)> Callback;
	float Weight = 1;

	size_t GetDependencyCount() { return m_Dependencies.size(); }
	Ref<JobNode> GetDependency(size_t index = 0)
	{
		return m_Dependencies[index];
	}


	template<typename T, typename... Args>
	void CreateBuffer(Args... args)
	{
		m_Buffer = new T(std::forward<Args>(args)...);
		m_BufferDestructor = [](void* ptr) { delete reinterpret_cast<T*>(ptr); };
	}

	template<typename T>
	T* Value()
	{
		return reinterpret_cast<T*>(m_Buffer);
	}

private:
	std::atomic_size_t m_RemainingDependencies = 1;

	std::atomic<JobStatus> m_Status;
	std::atomic_size_t m_ReturnCode;
	std::atomic_size_t m_RefCount;
	std::atomic<float> m_Progress;

	void* m_Buffer;
	JobDataDestructor m_BufferDestructor;
	std::vector<Ref<JobNode>> m_Dependant;
	std::vector<Ref<JobNode>> m_Dependencies;
	Ref<JobGraph> m_JobGraph = nullptr;
};

using JobNodeCallback = std::function<int(JobNode&)>;
using JobGraphCallback = std::function<int(JobGraph&)>;

struct JobGraph : public RefCount
{
	friend class JobPromise;
	friend class JobSystem;

	JobGraph() = default;
	JobGraph(const std::string& debugName) : m_DebugName(debugName) {}

	void AsyncJob(Ref<JobNode> node)
	{
		node->m_JobGraph = this;
		m_Jobs.push_back(node);
		m_JobsRunning++;
	}
	void OnFinished(JobGraphCallback callback)
	{
		m_FinishCallback = callback;
	}

	template<typename T> 
	T* Result(size_t index = 0)
	{
		return m_Jobs[index]->Value<T>();
	}
	template<typename T>
	T* DependencyResult(size_t graph = 0, size_t index = 0)
	{
		return m_DependencyGraphs[graph]->m_Jobs[index]->Value<T>();
	}

	const std::string& Name() { return m_DebugName; }
	std::vector<Ref<JobNode>> Jobs() { return m_Jobs; }

	void AsyncJobFinished();

private:
	std::string m_DebugName;
	JobGraphCallback m_FinishCallback;

	std::vector<Ref<JobNode>> m_Jobs;
	std::atomic<JobStatus> m_Status;
	std::atomic_size_t m_JobsRunning;
	std::vector<Ref<JobGraph>> m_DependantGraph;
	std::vector<Ref<JobGraph>> m_DependencyGraphs;
	JobSystem* m_JobSystem = nullptr;
};
