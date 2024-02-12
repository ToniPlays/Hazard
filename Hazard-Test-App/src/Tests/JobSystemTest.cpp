
#include "JobSystemTest.h"
#include "Hazard/Core/Application.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include <hzrpch.h>

static void Finalize(JobInfo& info)
{
	std::this_thread::sleep_for(1000ms);

	auto results = info.ParentGraph->GetResults<std::string>();
	std::string returnVal;

	for (auto& result : results)
		returnVal += result + "\n";

	info.Job->SetResult(returnVal);
}

static void AssetLoad(JobInfo& info)
{
	uint32_t count = info.ExecutionID;
	for (uint32_t i = 0; i < count; i++)
	{
		info.Job->Progress((float)i / (float)count);
		std::this_thread::sleep_for(1ms);
	}

	if (info.ExecutionID == 1)
		info.ParentGraph->Halt();

	std::string result = fmt::format("AssetLoad: Invocation {0}", info.ExecutionID);

	HZR_CORE_INFO(result);
	info.Job->SetResult(result);
}

static void Preprocess(JobInfo& info, int num)
{
	uint32_t count = num;
	for (uint32_t i = 0; i < count; i++)
	{
		info.Job->Progress((float)i / (float)count);
		std::this_thread::sleep_for(5ms);
	}

	std::vector<Ref<Job>> jobs;
	jobs.resize(100);

	for (uint32_t i = 0; i < jobs.size(); i++)
		jobs[i] = Ref<Job>::Create("AssetLoad", AssetLoad);

	info.ParentGraph->ContinueWith(jobs);
}

void JobGraphTest::Reset()
{
	delete m_JobSystem;
	m_JobSystem = nullptr;
}

void JobGraphTest::Init()
{
	Hazard::Application::Get().GetModule<Hazard::RenderContextManager>().GetWindow().SetWindowTitle(GetName());

	m_JobSystem = hnew JobSystem();

	m_JobSystem->Hook(JobSystem::Submit, [](Ref<JobGraph> graph) {
		HZR_INFO("Graph {} submitted", graph->GetName());
	});

	m_JobSystem->Hook(JobSystem::Finished, [](Ref<JobGraph> graph) {
		HZR_INFO("Graph {} finished", graph->GetName());
	});
	m_JobSystem->Hook(JobSystem::Failure, [](Ref<JobGraph> graph) {
		HZR_ERROR("Graph {} ({}) has failed, stopping", graph->GetName(), graph->GetCurrentStageInfo().Name);
	});

	m_JobSystem->Hook(JobSystem::Status, [](Ref<Thread> thread, ThreadStatus status) {
		switch (status)
		{
			case ThreadStatus::Failed:
			{
				HZR_ERROR("Thread {0} failed executing: {1}", thread->GetThreadID(), thread->GetCurrentJob()->GetName());
				break;
			}
		}
	});

	m_JobSystem->Hook(JobSystem::Message, [](Severity severity, const std::string& message) {
		switch (severity)
		{
			case Severity::Error:
				HZR_ERROR("JobSystem: {}", message);
				break;
			case Severity::Warning:
				HZR_WARN("JobSystem: {}", message);
				break;
		}
	});


	Ref<Job> loadingJob = Ref<Job>::Create("Preprocess", Preprocess, 100);
	Ref<Job> finalizeJob = Ref<Job>::Create("Finalize", Finalize);

	JobGraphInfo graphInfo = {
		.Name = "World load",
		.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
		.Stages = {
			{ "Preprocess", 0.2f, { loadingJob } },
			{ "Asset load", 0.6f, {} },
			{ "Finalize", 0.2f, { finalizeJob } }
		},
	};

	m_Graph = Ref<JobGraph>::Create(graphInfo);
	JobPromise promise = m_JobSystem->QueueGraph(m_Graph);

	promise.Then([](JobGraph& graph) {
		std::string result = graph.GetResult<std::string>();
		HZR_TRACE("Computed result: {0}", result);
	});
}

void JobGraphTest::Run()
{
	if (Input::IsKeyDown(Key::Enter))
		m_Graph->Continue();
}
void JobGraphTest::Terminate()
{
	m_JobSystem->Terminate();
}
