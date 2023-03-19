#pragma once

#include <Ref.h>
#include "Jobs.h"
#include "JobSystem.h"

class GraphStage;

class JobGraph : public RefCount
{
	friend class GraphStage;
	friend class JobSystem;
public:
	JobGraph(const std::string& name, uint32_t stageCount);

	uint32_t GetStageCount() const { return m_Stages.size(); }
	Ref<GraphStage> GetStage(uint32_t index) const { return m_Stages[index]; }
	Ref<GraphStage> GetNextStage();

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

private:
	void OnStageFinished();
private:
	std::string m_Name;
	std::vector<Ref<GraphStage>> m_Stages;
	std::atomic_uint32_t m_CurrentStage = 0;

	JobSystem* m_JobSystem;
};