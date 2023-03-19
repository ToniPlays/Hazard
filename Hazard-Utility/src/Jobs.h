#pragma once

#include <Ref.h>
#include <functional>
#include "GraphStage.h"
#include "JobGraph.h"

template<typename T>
class JobPromise
{
public:
	void Wait();

private:
		
};

class Job : public RefCount
{
	friend class JobSystem;
	friend class GraphStage;
	using JobCallback = std::function<void(Ref<Job>)>;

public:
	Job(JobCallback&& callback) : m_JobCallback(callback) {}

	const std::string& GetName() { return m_JobName; }
	void SetJobName(const std::string& name) { m_JobName = name; }

	float GetExecutionTime() { return m_ExecutionTime; }
	float WaitForUpdate() {
		m_Progress.wait(m_Progress);
		return m_Progress;
	}
	float GetProgress() { return m_Progress; }

	void Progress(float progress) 
	{ 
		m_Progress = progress; 
		m_Progress.notify_all();
	}

	Ref<JobGraph> GetJobGraph() 
	{ 
		if (!m_Stage) return nullptr;
		return m_Stage->GetGraph(); 
	}

private:
	JobCallback m_JobCallback;

	std::atomic<float> m_Progress = 0.0f;
	std::atomic<float> m_ExecutionTime = 0.0f;
	std::string m_JobName;

	Ref<GraphStage> m_Stage = nullptr;
};