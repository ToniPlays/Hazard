#pragma once

#include <Ref.h>
#include "Job.h"
#include "JobSystem.h"

class GraphStage;

class JobGraph : public RefCount
{
	friend class GraphStage;
	friend class JobSystem;
public:
	JobGraph(const std::string& name, uint32_t stageCount);

	size_t GetStageCount() const { return m_Stages.size(); }
	Ref<GraphStage> GetStage(uint32_t index) const { return m_Stages[index]; }
	Ref<GraphStage> GetNextStage();
	Ref<GraphStage> AddStage()
	{
		Ref<GraphStage> stage = Ref<GraphStage>::Create(m_Stages.size(), 0.0f);
		stage->m_JobGraph = this;
		m_Stages.push_back(stage);
		return stage;
	}

	void Execute();

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

	template<typename T>
	T GetResult()
	{
		return m_Stages[m_Stages.size() - 1]->GetResult<T>();
	}

private:
	void OnStageFinished();
private:
	std::string m_Name;
	std::vector<Ref<GraphStage>> m_Stages;
	std::atomic_uint32_t m_CurrentStage = 0;

	JobSystem* m_JobSystem;
};