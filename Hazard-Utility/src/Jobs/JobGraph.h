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
	std::vector<GraphStageInfo> Stages;
	uint32_t Flags = 0;
};

class JobGraph : public RefCount
{
	friend class GraphStage;
	friend class Job;

public:
	JobGraph(const JobGraphInfo& info);
	~JobGraph() = default;

	const std::string& GetName() const { return m_Info.Name; }
	uint32_t GetCurrentStageIndex() const { return m_CurrentStage; }
	uint32_t GetFlags() const { return m_Info.Flags; }

	const JobGraphInfo& GetInfo() const { return m_Info; }
	const GraphStageInfo& GetCurrentStageInfo() const 
	{ 
		return m_Info.Stages[Math::Min<uint64_t>(m_CurrentStage, m_Info.Stages.size() - 1)]; 
	}

	void Execute(JobSystem* system);
	void Halt();
	void Continue();

	void ContinueWith(const std::vector<Ref<Job>>& jobs);

	float GetProgress();

	void WaitUntilFinished() const
	{
		m_HasFinished.wait(false);
	}

	bool HasFinished() const
	{
		return m_HasFinished;
	}

	template<typename T>
	T GetResult() const
	{
		auto results = GetResults<T>();
		return results.size() > 0 ? results[0] : T();
	}

	template<typename T>
	std::vector<T> GetResults() const
	{
		if (m_CurrentStage < 1) return std::vector<T>();

		auto& stageJobs = m_Info.Stages[m_CurrentStage - 1].Jobs;
		std::vector<T> results;
		results.reserve(stageJobs.size());

		for (auto& job : stageJobs)
			results.push_back(job->GetResult<T>());

		return results;
	}

	void AddOnCompleted(const std::function<void(JobGraph&)>& callback)
	{
		m_OnCompleted.Add(callback);
	}

	template<typename T>
	static Ref<JobGraph> EmptyWithResult(T value)
	{
		Ref<Job> job = Ref<Job>::Create();
		job->SetResult(value);

		JobGraphInfo info = {};
		info.Stages = { { "", 1.0f, { job }} };
		auto graph = Ref<JobGraph>::Create(info);
		graph->m_HasFinished = true;
		return graph;
	}

private:
	void OnJobFinished(Ref<Job> job);
	void OnJobFailed(Ref<Job> job);
	void SubmitNextStage();
	
private:

	JobGraphInfo m_Info;
	std::atomic_uint32_t m_CurrentStage = 0;
	std::atomic_bool m_HasFinished = false;
	std::atomic_bool m_IsHalted = false;
	std::mutex m_StageMutex;

	JobSystem* m_JobSystem = nullptr;

	Callback<void(JobGraph&)> m_OnCompleted;
};
