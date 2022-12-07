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

struct JobNode : public RefCount
{
	friend class JobSystem;
	friend struct JobPromise;

	JobNode() = default;
	JobNode(const JobNode& other);

	std::string DebugName;
	std::function<int(JobNode&)> Callback;
	float Weight = 1;

	UID GetUID() { return m_Uid; }
	JobSystem* GetJobSystem() { return m_System; };

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

	UID m_Uid = 0;
	JobSystem* m_System = nullptr;
	std::atomic_size_t m_RemainingDependencies = 1;

	std::atomic<JobStatus> m_Status;
	std::atomic_size_t m_ReturnCode;
	std::atomic_size_t m_RefCount;
	std::atomic<float> m_Progress;

	void* m_Buffer;
	JobDataDestructor m_BufferDestructor;
	Ref<JobNode> m_Dependant = nullptr;
};

using JobCallback = std::function<int(JobNode&)>;

struct JobGraph : public RefCount
{
	JobGraph() = default;
	JobGraph(const std::string& debugName) : m_DebugName(debugName), m_Uid(UID()) {}

	void AsyncJob(Ref<JobNode> node)
	{
		m_Jobs.push_back(node);
	}
	void Then(JobCallback callback) {}
	const std::string& Name() { return m_DebugName; }
	UID GetUID() { return m_Uid; }
	std::vector<Ref<JobNode>> Jobs() { return m_Jobs; }

private:
	std::string m_DebugName;
	std::vector<Ref<JobNode>> m_Jobs;
	UID m_Uid;
};
