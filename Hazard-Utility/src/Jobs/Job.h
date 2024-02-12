#pragma once

#include <Ref.h>
#include <functional>
#include "JobException.h"
#include "JobFlags.h"
#include "Callback.h"

class JobGraph;
class Thread;
class Job;

struct JobInfo 
{
	Ref<Job> Job;
	Ref<Thread> Thread;
	Ref<JobGraph> ParentGraph;
	uint32_t StageIndex;
	uint32_t ExecutionID;
};

class Job : public RefCount
{
	friend class JobSystem;
	friend class JobGraph;
	using JobCallback = std::function<void(JobInfo&)>;

public:

	Job() = default;
	~Job();

	template<typename Fn, typename... Args>
	Job(const std::string& name, Fn&& callback, Args&&... args) : m_JobName(name)
	{
		m_JobCallback = std::bind(&callback, std::placeholders::_1, std::forward<Args>(args)...);
	}

	const std::string& GetName() const { return m_JobName; }
	JobStatus GetStatus() const { return m_Status; }

	void Execute(JobInfo& info);
	void Progress(float progress);

	float GetExecutionTime() const { return m_ExecutionTime; }
	float GetProgress() const { return m_Progress; }

	template<typename T>
	T GetResult() const
	{
		return m_Result ? *(T*)m_Result : T();
	}
	template<typename T>
	void SetResult(T value)
	{
		T* val = hnew T(value);
		m_Result = (void*)val;
		
		m_DestroyQueue.Add([result = val]() mutable {
			hdelete (T*)result;
		});
	}

private:

	JobCallback m_JobCallback;
	std::string m_JobName;

	std::atomic<float> m_Progress = 0.0f;
	std::atomic<float> m_ExecutionTime = 0.0f;
	
	uint32_t m_InvocationId = 0;
	JobGraph* m_JobGraph = nullptr;

	JobStatus m_Status = JobStatus::None;

	void* m_Result = nullptr;
	Callback<void()> m_DestroyQueue;
};
