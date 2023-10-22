
#include "JobSystemTest.h"
#include "Hazard/Core/Application.h"
#include "Hazard/RenderContext/RenderContextManager.h"

static void AssetLoad(JobInfo& info)
{
	uint32_t count = info.ExecutionID;
	for (uint32_t i = 0; i < count; i++)
	{
		info.Job->Progress((float)i / (float)count);
		std::this_thread::sleep_for(1ms);
	}
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
	{
		jobs[i] = Ref<Job>::Create("Job", AssetLoad);
	}

	info.NextStage->QueueJobs(jobs);
}

static std::string GetStatus(const ThreadStatus& status)
{
	switch (status)
	{
		case ThreadStatus::None:		return "None";
		case ThreadStatus::Waiting:		return "Waiting";
		case ThreadStatus::Executing:	return "Executing";
		case ThreadStatus::Terminated:	return "Terminated";
	}
	return "";
}

void JobGraphTest::Reset()
{
	delete m_JobSystem;
	m_Graph = nullptr;
	m_JobSystem = nullptr;
}

void JobGraphTest::Init()
{
#ifdef HZR_PLATFORM_MACOS
	std::filesystem::current_path("/users/ToniSimoska/Hazard/HazardLauncher");
#endif

	Hazard::Application::Get().GetModule<Hazard::RenderContextManager>().GetWindow().SetWindowTitle(GetName());

	m_JobSystem = new JobSystem();

	Ref<Job> loadingJob = hnew Job("World loading", Preprocess, 100);
	m_Graph = Ref<JobGraph>::Create("World loader", 2);

	Ref<GraphStage> loading = m_Graph->GetStage(0);
	loading->SetWeight(0.40f);
	loading->QueueJobs({ loadingJob });
	m_Graph->GetStage(1)->SetWeight(0.60f);

	m_JobSystem->QueueGraph(m_Graph);
}

void JobGraphTest::Run()
{
	if (!m_Graph) return;

	static float progress = 0.0f;

	static Timer timer;
	if (!m_Graph->HasFinished())
	{
		progress = m_Graph->GetProgress();
		std::cout << fmt::format("{} progress: {}%", m_Graph->GetName(), progress * 100.0f) << std::endl;
	}
	else
	{
		std::cout << fmt::format("Total time {} ms", timer.ElapsedMillis()) << std::endl;
		m_Graph = nullptr;
	}
}
void JobGraphTest::Terminate()
{
	m_JobSystem->Terminate();
}