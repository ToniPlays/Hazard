#pragma once

#include <Ref.h>
#include "JobFlags.h"
#include "Job.h"

class JobGraph;

class GraphStage : public RefCount
{
	friend class JobSystem;
	friend class JobGraph;
	friend class Job;

public:
	GraphStage(uint32_t index, float weight, uint32_t flags = 0, const std::string& name = "") : m_StageIndex(index), m_Weight(weight), m_Flags(flags), m_Name(name) {}
	~GraphStage();

	float GetWeight() const { return m_Weight; }
	void SetWeight(float weight) { m_Weight = weight; }
	const std::vector<Ref<Job>>& GetJobs() { return m_Jobs; }
	float GetProgress();
	uint32_t GetFlags() { return m_Flags; }

	const std::string& GetName() { return m_Name; }

	uint32_t GetStageIndex() const { return m_StageIndex; }
    Ref<GraphStage> GetGraphStage(uint32_t index);

	void QueueJobs(const std::vector<Ref<Job>>& jobs);

	Ref<JobGraph> GetGraph();

	template<typename T>
	std::vector<T> GetJobResults()
	{
		std::vector<T> result;
		result.reserve(m_Jobs.size());

		for (auto& job : m_Jobs)
			result.push_back(job->GetResult<T>());
		return result;
	}

private:
	void OnJobFinished(Ref<Job> job);

private:

	float m_Weight = 1.0f;
	std::vector<Ref<Job>> m_Jobs;
	Ref<JobGraph> m_JobGraph;

	std::string m_Name;
	std::mutex m_JobMutex;
	std::atomic_uint64_t m_JobCount;

	uint32_t m_Flags = JOB_FLAGS_SUCCEEDED;
	uint32_t m_StageIndex = 0;
};
