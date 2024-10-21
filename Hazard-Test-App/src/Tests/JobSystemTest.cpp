
#include "JobSystemTest.h"
#include "Hazard/Core/Application.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include <hzrpch.h>

void JobGraphTest::Reset()
{
	m_JobSystem.release();
}

void JobGraphTest::Init()
{
	Hazard::Application::Get().GetModule<Hazard::RenderContextManager>().GetWindow().SetWindowTitle(GetName());

    m_JobSystem = CreateScope<JobSystem>();
    InitializeHooks();
    
    Ref<Job> preprocess = Job::Lambda("Preload", [](JobInfo& info) {
        HZR_INFO("Executing function");
        std::this_thread::sleep_for(2500ms);
        HZR_INFO("Finished executing function");
        
        //throw JobException("Failed");
        
        for(uint32_t i = 0; i < 50; i++)
        {
            info.ContinueWith(Job::Lambda(fmt::format("Asset {0} load", i), [](JobInfo& info) {
                std::this_thread::sleep_for(200ms);
                
            }));
        }
    });
    
    Ref<Job> finalize = Job::Lambda("Finalize", [](JobInfo& info) {
        HZR_INFO("Finalized");
    });

	JobGraphInfo graphInfo = {
		.Name = "World load test",
		.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
		.Stages = {
			{ "Preprocess", 0.2f, { preprocess } },
            { "Asset load", 0.7f, { } },
            { "Finalize", 0.1f , { finalize } }
		},
	};

	m_Graph = Ref<JobGraph>::Create(graphInfo);
	Promise<uint32_t> promise = m_JobSystem->Submit<uint32_t>(m_Graph);
    
    promise.ContinueWith([]() {
        HZR_INFO("Job graph finished");
    }).Catch([](const JobException& exception) {
        HZR_ERROR("Grahp Error: {} ", exception.what());
    }).Wait();
    
    std::cout << "Finished" << std::endl;
}

void JobGraphTest::Run()
{
    m_JobSystem->Update();
    m_JobSystem->WaitForJobsToFinish();
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
