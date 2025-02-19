#pragma once

#include <Ref.h>
#include <functional>
#include "JobException.h"
#include "JobFlags.h"
#include "Utility/Callback.h"
#include "Utility/Coroutine.h"

#include "spdlog/fmt/fmt.h"

class JobGraph;
class Thread;
struct JobInfo;

class Job : public RefCount
{
	friend class JobSystem;
	friend class JobGraph;
	using JobCallback = std::function<Coroutine(JobInfo)>;

public:

	~Job()
	{
		if (!m_JobCoroutine.Done())
			HZR_ASSERT(false, "Exit without completion");

		m_DeleteCallback.Invoke();
		m_DeleteCallback.Clear();
		s_JobsAlive--;

		m_JobCoroutine.Destroy();
	}

	const std::string& GetName() const { return m_JobName; }
	JobStatus GetStatus() const { return m_Status; }
	const Coroutine& GetCoroutine() const { return m_JobCoroutine; }

	void Execute(JobInfo info);
	void Finish();
	void Progress(float progress);

	float GetExecutionTime() const { return m_ExecutionTime; }
	float GetProgress() const { return m_Progress; }

	std::optional<JobException> GetException() const { return m_Exception; }

	template<typename T>
	void SetResult(T value)
	{
		m_DeleteCallback.Invoke();
		m_DeleteCallback.Clear();

		T* val = hnew T(value);
		m_ResultBuffer = (void*)val;

		m_DeleteCallback.Add([container = (void*)val]() {
			((T*)container)->~T();
			hdelete container;
			});
	}
	template<typename T>
	T GetResult() const
	{
		if (!m_ResultBuffer)
			return T();
		return *(T*)m_ResultBuffer;
	}

public:
	template<typename Fn, typename... Args>
	static Ref<Job> Create(const std::string& name, Fn&& callback, Args&&... args)
	{
		return hnew Job(name, callback, args...);
	}

	static Ref<Job> Lambda(const std::string& name, JobCallback&& callback)
	{
		Ref<Job> job = Create();
		job->m_JobName = name;
		job->m_JobCallback = callback;
		return job;
	}
	static Ref<Job> Create()
	{
		return hnew Job();
	}

	static uint32_t GetJobsAlive() {
		return s_JobsAlive;
	}

private:
	Job() {
		s_JobsAlive++;
	};

	template<typename Fn, typename... Args>
	Job(const std::string& name, Fn&& callback, Args&&... args) : m_JobName(name)
	{
		s_JobsAlive++;
		m_JobCallback = std::bind(std::forward<Fn>(callback), std::placeholders::_1, std::forward<Args>(args)...);
	}

private:
	Coroutine m_JobCoroutine;
	std::string m_JobName;

	std::atomic<float> m_Progress = 0.0f;
	std::atomic<float> m_ExecutionTime = 0.0f;
	JobStatus m_Status = JobStatus::None;

	uint32_t m_InvocationId = 0;

	std::optional<JobException> m_Exception;
	JobGraph* m_JobGraph = nullptr;
	void* m_ResultBuffer = nullptr;
	JobCallback m_JobCallback;

	Callback<void()> m_DeleteCallback;

	inline static std::atomic_uint32_t s_JobsAlive = 0;
};

struct JobInfo
{
	friend class Job;

	Job* Current;
	JobGraph* Graph;
	uint32_t StageIndex;
	uint32_t ExecutionID;

	template<typename T>
	void Result(T value)
	{
		Current->SetResult<T>(value);
	};

	void ContinueWith(const std::vector<Ref<Job>>& jobs);
};
