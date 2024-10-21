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
	Ref<Job> Current;
	Ref<Thread> Thread;
	Ref<JobGraph> Graph;
	uint32_t StageIndex;
	uint32_t ExecutionID;
    
    void ContinueWith(Ref<Job> job);
};

class Job : public RefCount
{
	friend class JobSystem;
	friend class JobGraph;
	using JobCallback = std::function<void(JobInfo&)>;

public:
	const std::string& GetName() const { return m_JobName; }
	JobStatus GetStatus() const { return m_Status; }

	void Execute(JobInfo& info);
	void Progress(float progress);

	float GetExecutionTime() const { return m_ExecutionTime; }
	float GetProgress() const { return m_Progress; }
    
    std::optional<JobException> GetException() const { return m_Exception; }
    
    template<typename Fn, typename... Args>
    static Ref<Job> Create(const std::string& name, Fn&& callback, Args&&... args)
    {
        return hnew Job(name, callback, args...);
    }
    
    static Ref<Job> Lambda(const std::string& name, JobCallback&& callback)
    {
        Ref<Job> job = Create();
        job->m_JobName = name;
        job->m_JobCallback = callback;
        return job;
    }
    static Ref<Job> Create()
    {
        return hnew Job();
    }
    
private:
    
    Job() = default;
    
    template<typename Fn, typename... Args>
    Job(const std::string& name, Fn&& callback, Args&&... args) : m_JobName(name)
    {
        m_JobCallback = std::bind(&callback, std::placeholders::_1, std::forward<Args>(args)...);
    }

private:

	JobCallback m_JobCallback;
	std::string m_JobName;

	std::atomic<float> m_Progress = 0.0f;
	std::atomic<float> m_ExecutionTime = 0.0f;
	
	uint32_t m_InvocationId = 0;
	JobGraph* m_JobGraph = nullptr;

	JobStatus m_Status = JobStatus::None;
    std::optional<JobException> m_Exception;
};
