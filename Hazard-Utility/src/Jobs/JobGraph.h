#pragma once

#include <Ref.h>
#include "JobFlags.h"
#include "Callback.h"
#include "Job.h"
#include <functional>
#include <MathCore.h>

class JobSystem;

struct GraphStageInfo 
{
	std::string Name;
	float Weight;
	std::vector<Ref<Job>> Jobs;
};

struct JobGraphInfo
{
	std::string Name;
	uint32_t Flags = 0;
	std::vector<GraphStageInfo> Stages;
};

class JobGraph : public RefCount
{
	friend class GraphStage;
	friend class Job;
    friend class JobPromise;
public:
	JobGraph(const JobGraphInfo& info);
	~JobGraph() = default;

	const std::string& GetName() const { return m_Info.Name; }
	uint32_t GetFlags() const { return m_Info.Flags; }

	const JobGraphInfo& GetInfo() const { return m_Info; }
    const std::string& GetStageName() const { return m_Info.Stages[m_StageIndex].Name; }
    
	bool SubmitJobs(JobSystem* system);
	void ContinueWith(const std::vector<Ref<Job>>& jobs);
    bool Wait() const {
        if(!m_JobSystem) return false;
        m_HasFinished.wait(false);
        return true;
    }
    
    void AddOnFailed(const std::function<void(JobException)>&& callback)
    {
        m_OnFailedCallback.Add(callback);
    }
    
	float GetProgress();

	template<typename T>
	T GetResult() const
	{
        return T();
	}

	template<typename T>
	static Ref<JobGraph> EmptyWithResult(T value)
	{
		Ref<Job> job = Job::Create();
		
		JobGraphInfo info = {};
		info.Stages = { { "", 1.0f, { job }} };
		auto graph = Ref<JobGraph>::Create(info);
		return graph;
	}

private:
	void OnJobFinished(Ref<Job> job);
	void OnJobFailed(Ref<Job> job);
	void SubmitNextStage();
	
private:
    std::atomic_bool m_HasFinished = false;
    std::atomic_bool m_Failed = false;
    std::atomic_uint32_t m_RunningJobs = 0;
    
	JobGraphInfo m_Info;
	JobSystem* m_JobSystem = nullptr;
    
    uint32_t m_StageIndex = 0;
    
    Callback<void(const JobException&)> m_OnFailedCallback;
};
