#pragma once

#include <Ref.h>
#include <functional>
#include "GraphStage.h"
#include "JobGraph.h"


using JobCallback = std::function<void(Ref<Job>)>;

class Job : public RefCount
{
	friend class JobSystem;
	friend class GraphStage;

public:
	Job() = delete;

	template<typename Fn, typename... Args>
	Job(Fn&& callback, Args&&... args)
	{
		m_JobCallback = std::bind(&callback, std::placeholders::_1, std::forward<Args>(args)...);
	}

	const std::string& GetName() { return m_JobName; }
	void SetJobName(const std::string& name) { m_JobName = name; }

	const std::string& GetTag() { return m_JobTag; }
	void SetJobTag(const std::string& name) { m_JobTag = name; }

	void Execute();

	float GetExecutionTime() { return m_ExecutionTime; }
	float WaitForUpdate()
	{
		m_Progress.wait(m_Progress);
		return m_Progress;
	}
	float GetProgress() { return m_Progress; }

	void Progress(float progress)
	{
		m_Progress = progress;
		m_Progress.notify_all();
	}

	template<typename T>
	T GetInput()
	{
		if (!m_Stage)
			return T();

		if (!m_Stage->GetGraph()) 
			return T();

		uint32_t index = m_Stage->GetStageIndex() - 1;
		Ref<GraphStage> stage = m_Stage->GetGraph()->GetStage(index);
		return stage->GetResult<T>();
	}

	Ref<GraphStage> GetStage() { return m_Stage; }

	Ref<JobGraph> GetJobGraph()
	{
		if (!m_Stage) return nullptr;
		return m_Stage->GetGraph();
	}

private:
	JobCallback m_JobCallback;

	std::atomic<float> m_Progress = 0.0f;
	std::atomic<float> m_ExecutionTime = 0.0f;
	std::string m_JobTag;
	std::string m_JobName;

	Ref<GraphStage> m_Stage = nullptr;
};