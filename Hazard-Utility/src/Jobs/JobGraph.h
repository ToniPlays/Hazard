#pragma once

#include <Ref.h>
#include "JobFlags.h"
#include "GraphStage.h"

class JobSystem;
class GraphStage;
class Job;

class JobGraph : public RefCount
{
	friend class GraphStage;
	friend class JobSystem;
public:
	JobGraph(const std::string& name, uint32_t stageCount);

	const std::string& GetName() const { return m_Name; }
	uint32_t GetCurrentStageIndex() const { return m_CurrentStage; }
	Ref<GraphStage> GetCurrentStage() const { return m_Stages[m_CurrentStage]; }
	uint64_t GetStageCount() const { return m_Stages.size(); }
	Ref<GraphStage> GetStage(uint32_t index) const { return m_Stages[index]; }
	const std::vector<Ref<GraphStage>>& GetStages() const { return m_Stages; }
	uint32_t GetFlags() { return m_Flags; }

	Ref<GraphStage> GetNextStage();
	Ref<GraphStage> GetPreviousStage();
	Ref<GraphStage> AddStage();

	void CombineStages(Ref<JobGraph> graph, uint32_t offset = 0);

	Ref<JobGraph> Execute();

	float GetProgress();

	bool HasFinished() 
	{
		return m_CurrentStage >= m_Stages.size();
	}

	void Wait()
	{
		while (!HasFinished())
			m_CurrentStage.wait(m_CurrentStage);
	}

	Buffer GetResult();
	std::vector<Buffer> GetResults();

private:
	void OnStageFinished(Ref<GraphStage> stage);

private:
	std::string m_Name;
	std::vector<Ref<GraphStage>> m_Stages;
	std::atomic_uint32_t m_CurrentStage = 0;
	uint32_t m_Flags = JOB_FLAGS_SUCCEEDED;

	JobSystem* m_JobSystem = nullptr;
};
