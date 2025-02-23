#pragma once

#include "JobFlags.h"

#include "Ref.h"
#include "Utility/Awaitable.h"
#include "JobGraph.h"

#include "spdlog/fmt/fmt.h"

class Job;
class JobSystem;

template<typename T>
class Promise : public Awaitable<std::vector<T>>
{
	friend class JobSystem;

public:
	Promise() = default;

	const std::string& GetJobName() const
	{
		return m_JobGraph->GetName();
	}

	bool Succeeded() const
	{
		return !m_JobGraph->DidFail();
	}

	Promise<T> Wait() const
	{
		if (m_JobGraph)
			m_JobGraph->Wait();
		return *this;
	}

	Promise<T> ContinueWith(const std::function<void(const std::vector<T>&)>& callback) {
		if (m_JobGraph)
			m_JobGraph->AddOnFinished([callback, graph = m_JobGraph]() {
				callback(graph->template GetResults<T>());
				});

		return *this;
	}

	Promise<T> Catch(const std::function<void(const JobException&)>& callback) {
		if (m_JobGraph)
			m_JobGraph->AddOnFailed(callback);
		return *this;
	}

	std::vector<T> GetResults() override
	{
		if (m_JobGraph)
			return m_JobGraph->GetResults<T>();
		else return std::vector<T>();
	}

	bool AwaitReady() override
	{
		return m_JobGraph ? m_JobGraph->HasFinished() : false;
	}

    void OnSuspend() override
	{
		if (!m_JobGraph)
		{
			this->Resolve();
			return;
		}
        
		ContinueWith([instance = this](const auto&) mutable {
			instance->Resolve();
		});

		Catch([instance = this](const JobException& err) {
			std::cout << fmt::format("Promise failed with: {0}", err.what()) << std::endl;
			instance->Resolve();
			});
	}

	static Promise<T> Create(Ref<JobGraph> graph)
	{
		return Promise<T>(graph);
	}

	operator bool() { return m_JobGraph; }

private:
	Promise(Ref<JobGraph> graph) : m_JobGraph(graph) {}

private:
	Ref<JobGraph> m_JobGraph = nullptr;
};
