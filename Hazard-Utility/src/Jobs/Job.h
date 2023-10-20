#pragma once

#include <Ref.h>
#include <functional>
#include "JobException.h"
#include "JobFlags.h"

class JobGraph;
class GraphStage;

class Job : public RefCount
{
	friend class JobSystem;
	friend class GraphStage;

	using JobCallback = std::function<void(Ref<Job>)>;

public:

	Job() = delete;
	~Job();

	template<typename Fn, typename... Args>
	Job(const std::string& name, Fn&& callback, Args&&... args) : m_JobName(name)
	{
		m_JobCallback = std::bind(&callback, std::placeholders::_1, std::forward<Args>(args)...);
	}

	const std::string& GetName() { return m_JobName; }
	const std::string& GetTag() { return m_JobTag; }
	JobStatus GetStatus() { return m_Status; }

	void SetJobTag(const std::string& name) { m_JobTag = name; }

	void Execute();
	void Progress(float progress);

	float GetExecutionTime() { return m_ExecutionTime; }
	float WaitForUpdate();
	float GetProgress() { return m_Progress; }

	Ref<GraphStage> GetStage() { return m_Stage; }
	Ref<JobGraph> GetJobGraph();

	Buffer GetResult() { return m_ResultBuffer; }
	void SetResult(void* data, uint64_t size, uint64_t offset = 0);

private:
	JobCallback m_JobCallback;

	std::atomic<float> m_Progress = 0.0f;
	std::atomic<float> m_ExecutionTime = 0.0f;
	
	std::string m_JobTag;
	std::string m_JobName;
	JobStatus m_Status;

	Buffer m_ResultBuffer;

	Ref<GraphStage> m_Stage = nullptr;
};
