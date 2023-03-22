#pragma once

#include <Ref.h>

class Job;
class JobGraph;

class GraphStage : public RefCount
{
	friend class JobSystem;
	friend class JobGraph;
	friend class Job;
public:
	GraphStage(uint32_t index, float weight) : m_StageIndex(index), m_Weight(weight) {}
	~GraphStage();

	float GetWeight() const { return m_Weight; }
	void SetWeight(float weight) { m_Weight = weight; }

	const std::vector<Ref<Job>>& GetJobs() { return m_Jobs; }

	void QueueJobs(const std::vector<Ref<Job>>& jobs);
	float GetProgress();

	uint32_t GetStageIndex() const { return m_StageIndex; }

	Ref<JobGraph> GetGraph()
	{
		if (!m_JobGraph) return nullptr;
		return m_JobGraph;
	}
	template<typename T>
	T GetResult()
	{
		if (!m_ResultBuffer.Data)
			return T();

		return m_ResultBuffer.Read<T>();
	}

	template<typename T>
	void SetResult(T result)
	{
		m_ResultBuffer.Release();
		m_ResultBuffer.Allocate(sizeof(T));
		m_ResultBuffer.Write(&result, sizeof(T));
	}

private:
	void OnJobFinished();

private:
	float m_Weight = 1.0f;
	std::vector<Ref<Job>> m_Jobs;
	Ref<JobGraph> m_JobGraph;

	std::mutex m_JobMutex;
	std::atomic_uint32_t m_JobCount;

	Buffer m_ResultBuffer;
	uint32_t m_StageIndex = 0;
};