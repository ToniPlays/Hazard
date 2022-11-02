#pragma once

#include <thread>
#include <functional>
#include "Profiling/MemoryDiagnostic.h"
#include "UtilityCore.h"

#define MULTI_THREAD 1


class Thread
{
	friend class ThreadPool;
public:
	Thread(ThreadPool* pool) { m_ThreadPool = pool; };

	void OnCompletionHandler(std::function<void(const Thread& thread)> function)
	{
		m_OnCompletionHandler = function;
	}

	void Dispatch(std::function<void()> function)
	{
		HZR_ASSERT(m_HasCompleted, "Thread not completed");
		m_HasCompleted = false;

#if MULTI_THREAD
		std::function<void()> fn = [=]() {

			auto& startTime = std::chrono::high_resolution_clock::now();
			function();

			m_ExecutionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count() * 0.001f * 0.001f;


			if (m_OnCompletionHandler)
				m_OnCompletionHandler(*this);

			m_HasCompleted = true;
		};
		m_Thread = std::thread(fn);
#else
		auto& startTime = std::chrono::high_resolution_clock::now();
		function();

		m_ExecutionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count() * 0.001f * 0.001f;


		if (m_OnCompletionHandler)
			m_OnCompletionHandler(*this);

		m_HasCompleted = true;
#endif
	}

	void WaitForCompletion()
	{
		if (m_HasCompleted) return;

		m_Thread.join();

		if (m_OnCompletionHandler)
			m_OnCompletionHandler(*this);
	}
	bool HasCompleted() const
	{
		return m_HasCompleted;
	}
	float GetExecutionTime() const
	{
		return m_ExecutionTime;
	}

private:
	ThreadPool* m_ThreadPool;
	std::thread m_Thread;
	std::function<void(const Thread& thread)> m_OnCompletionHandler;
	bool m_HasCompleted = true;

	//Time in milliseconds
	double m_ExecutionTime;
};

class ThreadPool
{
public:
	ThreadPool(size_t size = 0)
	{
		m_Threads.resize(size);
	};
	~ThreadPool()
	{
		for (auto& thread : m_Threads)
			thread->WaitForCompletion();
		for (auto& thread : m_Threads)
			hdelete thread;
	}
	Thread& GetThread()
	{
		Thread* thread = hnew Thread(this);
		m_Threads.push_back(thread);
		return *thread;
	}

private:
	std::vector<Thread*> m_Threads;
};