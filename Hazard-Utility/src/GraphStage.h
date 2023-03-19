#pragma once

#include <Ref.h>

class Job;
class JobGraph;

class GraphStage : public RefCount
{
	friend class JobSystem;
	friend class JobGraph;
public:
	GraphStage(float weight) : m_Weight(weight) {}

	float GetWeight() const { return m_Weight; }
	void SetWeight(float weight) { m_Weight = weight; }

	void QueueJobs(const std::vector<Ref<Job>>& jobs);

	float GetProgress();

	Ref<JobGraph> GetGraph()
	{
		if (!m_JobGraph) return nullptr;
		return m_JobGraph;
	}

private:
	void OnJobFinished();

private:
	float m_Weight = 1.0f;
	std::vector<Ref<Job>> m_Jobs;
	Ref<JobGraph> m_JobGraph;

	std::mutex m_JobMutex;
	std::atomic_uint32_t m_JobCount;
};