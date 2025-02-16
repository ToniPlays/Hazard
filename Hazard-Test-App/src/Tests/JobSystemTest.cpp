
#include "JobSystemTest.h"
#include "Hazard/Core/Application.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include <hzrpch.h>

static Ref<JobGraph> GetDummyGraph()
{
	Ref<Job> job = Job::Lambda("Dumb", [](JobInfo& info) -> Coroutine {
        std::this_thread::sleep_for(2000ms);
        info.Result(234.04332f);
		co_return;
	});

	JobGraphInfo graphInfo = {
		.Name = "World load test",
		.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
		.Stages = {
			{ "Preprocess", 0.2f, { job } },
		},
	};
	return Ref<JobGraph>::Create(graphInfo);
}

void JobGraphTest::Reset()
{
	m_JobSystem.release();
}

void JobGraphTest::Init()
{
	Hazard::Application::Get().GetModule<Hazard::RenderContextManager>().GetWindow().SetWindowTitle(GetName());

	m_JobSystem = CreateScope<JobSystem>();
	InitializeHooks();

	Ref<Job> preprocess = Job::Lambda("Preload", [&](JobInfo info) -> Coroutine {
		HZR_INFO("Executing function");
		std::vector<float> results = co_await m_JobSystem->Submit<float>(GetDummyGraph());
        for(float f : results)
        {
            HZR_INFO(f);
        }
		HZR_INFO("Finished executing function");
        co_return;
	});

	JobInfo info = {};
	preprocess->Execute(info);
	
	JobGraphInfo graphInfo = {
		.Name = "World load test",
		.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
		.Stages = {
			{ "Preprocess", 0.2f, { preprocess } },
		},
	};

	//m_Graph = Ref<JobGraph>::Create(graphInfo);
	//Promise<uint32_t> promise = m_JobSystem->Submit<uint32_t>(m_Graph);

	//promise.Wait();
}

void JobGraphTest::Run()
{
	m_JobSystem->Update();
	m_JobSystem->WaitForJobsToFinish();
	std::this_thread::sleep_for(60000ms);
}

void JobGraphTest::Terminate()
{
	m_JobSystem->Terminate();
}

void JobGraphTest::InitializeHooks()
{
	m_JobSystem->Hook(JobSystemHook::Submit, [](Ref<JobGraph> graph) {
		HZR_INFO("Graph {} submitted", graph->GetName());
		});

	m_JobSystem->Hook(JobSystemHook::Finished, [](Ref<JobGraph> graph) {
		HZR_INFO("Graph {} finished", graph->GetName());
		});
	m_JobSystem->Hook(JobSystemHook::Failure, [](Ref<JobGraph> graph) {
		HZR_ERROR("Graph {} ({}) has failed, stopping", graph->GetName(), graph->GetStageName());
		});

	m_JobSystem->Hook(JobSystemHook::Status, [](Ref<Thread> thread, ThreadStatus status) {
		switch (status)
		{
			case ThreadStatus::Failed:
			{
				HZR_ERROR("Thread {0} failed executing: {1}", thread->GetThreadID(), thread->GetCurrentJob()->GetName());
				break;
			}
			case ThreadStatus::Executing:
			{
				HZR_INFO("Thread {0} started executing job {1}", thread->GetThreadID(), thread->GetCurrentJob()->GetName());
				break;
			}
			default: break;
		}
		});

	m_JobSystem->Hook(JobSystemHook::Message, [](Severity severity, const std::string& message) {
		switch (severity)
		{
			case Severity::Error:
				HZR_ERROR("JobSystem Error: {0}", message);
				break;
			case Severity::Warning:
				HZR_WARN("JobSystem Warning: {0}");
				break;
			default: break;
		}
		});
}
