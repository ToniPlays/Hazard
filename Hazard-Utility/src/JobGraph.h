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
	Waiting = 1,
	Executing = 2,
	Done = 3,
	Error = 4
};

struct JobGraph;

struct JobNode : public RefCount
{
	friend struct JobGraph;
	friend class JobSystem;
	friend struct JobPromise;

	JobNode() = default;
	JobNode(const JobNode& other);

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

	Ref<JobGraph> GetGraph() { return m_JobGraph; }
	void JobStarted()
	{
		m_RemainingDependencies = m_Dependencies.size();
		m_Status = JobStatus::Executing;
		m_Status.notify_all();
	}
	void JobFinished();
	void SetProgress(float progress);

public:
	std::string DebugName;
	std::function<int(JobNode&)> Callback;
	float Weight = 1.0f;

private:
	std::atomic_size_t m_RemainingDependencies = 0;

	std::atomic<JobStatus> m_Status;
	std::atomic_size_t m_ReturnCode;
	std::atomic_size_t m_RefCount;
	std::atomic<float> m_Progress = 0;

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
	friend class JobSystem;
	friend struct JobPromise;

	JobGraph() = default;
	JobGraph(const std::string& debugName) : m_DebugName(debugName) {}

	void PushNode(Ref<JobNode> node);	
	void Then(Ref<JobNode> node);
	void OnFinished(JobGraphCallback callback)
	{
		m_FinishCallback = callback;
	}

	template<typename T, typename... Args>
	void CreateBuffer(Args... args)
	{
		m_Buffer = new T(std::forward<Args>(args)...);
		m_BufferDestructor = [](void* ptr) { delete reinterpret_cast<T*>(ptr); };
	}

	template<typename T>
	T* Result() const
	{
		return reinterpret_cast<T*>(m_Buffer);
	}
	template<typename T>
	T* DependencyResult(size_t graph = 0)
	{
		return reinterpret_cast<T*>(m_DependencyGraphs[graph]->m_Buffer);
	}

	const std::string& Name() { return m_DebugName; }
	std::vector<Ref<JobNode>> Jobs() { return m_AllJobs; }
	JobSystem& GetSystem() { return *m_JobSystem; }

	void AsyncJobFinished(Ref<JobNode> node);
	void UpdateProgress();

private:
	std::string m_DebugName;
	JobGraphCallback m_FinishCallback;

	std::atomic<JobStatus> m_Status;
	std::atomic_size_t m_JobsRunning = 0;
	std::atomic<float> m_Progress;

	std::vector<Ref<JobNode>> m_AllJobs;
	std::vector<Ref<JobGraph>> m_DependantGraph;
	std::vector<Ref<JobGraph>> m_DependencyGraphs;
	JobSystem* m_JobSystem = nullptr;

	void* m_Buffer = nullptr;
	JobDataDestructor m_BufferDestructor = nullptr;
};
