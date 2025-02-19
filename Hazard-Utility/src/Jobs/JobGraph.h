#pragma once

#include <Ref.h>
#include "JobFlags.h"
#include "Utility/Callback.h"
#include "Job.h"
#include <functional>
#include "Math/MathCore.h"

#include "spdlog/fmt/fmt.h"
#include <stacktrace>

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
    friend class JobSystem;
    friend class JobPromise;

public:
	JobGraph(const JobGraphInfo& info);
    ~JobGraph() {
        s_GraphsAlive--;
        m_HasFinished = true;
        m_HasFinished.notify_all();
    };

	const std::string& GetName() const { return m_Info.Name; }
	uint32_t GetFlags() const { return m_Info.Flags; }

    bool HasFinished() const { return m_HasFinished; }
    bool DidFail() const { return m_Failed; }
	const JobGraphInfo& GetInfo() const { return m_Info; }
    const std::string& GetStageName() const { return m_Info.Stages[Math::Min<uint32_t>(m_StageIndex, m_Info.Stages.size() - 1)].Name; }
    
	bool SubmitJobs(JobSystem* system);
	void ContinueWith(const std::vector<Ref<Job>>& jobs);
    bool Wait() const 
	{
        m_HasFinished.wait(false);
        return true;
    }
    
    template<typename T>
    std::vector<T> GetResults() const
    {
        if (m_StageIndex < 1) return std::vector<T>();

        auto& stageJobs = m_Info.Stages[m_StageIndex - 1].Jobs;
        std::vector<T> results;
        results.reserve(stageJobs.size());

        for (auto& job : stageJobs)
            results.push_back(job->GetResult<T>());

        return results;
    }
    
    void AddOnFinished(const std::function<void()>&& callback)
    {
        if (m_HasFinished)
            callback();
        else m_OnFinishedCallback.Add(callback);
    }
    
    void AddOnFailed(const std::function<void(JobException)>&& callback)
    {
        m_OnFailedCallback.Add(callback);
    }
    
	float GetProgress();


	template<typename T>
	static Ref<JobGraph> EmptyWithResult(const std::vector<T> value)
	{
        std::vector<Ref<Job>> jobs(value.size(), Job::Create());
        for (uint32_t i = 0; i < jobs.size(); i++)
        {
            jobs[i]->SetResult(value[i]);
        }
		
		JobGraphInfo info = {};
		info.Stages = { { "", 1.0f, jobs } };

		auto graph = Ref<JobGraph>::Create(info);
        graph->m_HasFinished = true;
        graph->m_StageIndex = 1;
		return graph;
	}

    static uint32_t GetGraphsAlive() {
        return s_GraphsAlive;
    }

private:
	void OnJobFinished(Ref<Job> job);
	void OnJobFailed(Ref<Job> job);
	void SubmitNextStage();
	
private:
    std::atomic_bool m_HasFinished = false;
    std::atomic_bool m_Failed = false;
    std::atomic_uint32_t m_RunningJobs = 0;
	std::mutex m_JobMutex;
    
	JobGraphInfo m_Info;
	JobSystem* m_JobSystem = nullptr;
    
    uint32_t m_StageIndex = 0;
    
    Callback<void()> m_OnFinishedCallback;
    Callback<void(const JobException&)> m_OnFailedCallback;

    inline static std::atomic_uint32_t s_GraphsAlive = 0;
};
