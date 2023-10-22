#pragma once

#include <iostream>
#include <Jobs.h>

#include <spdlog/fmt/fmt.h>
#include <Profiling/Timer.h>
#include <Random.h>
#include <Jobs/JobSystem.h>

namespace JobSystemTest
{
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
		jobs.resize(1000);
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

	static void Run()
	{
#ifdef HZR_PLATFORM_MACOS
		std::filesystem::current_path("/users/ToniSimoska/Hazard/HazardLauncher");
#endif
		JobSystem jobSystem;
		Ref<Job> loadingJob = hnew Job("World loading", Preprocess, 100);
		Ref<JobGraph> graph = Ref<JobGraph>::Create("World loader", 2);

		Ref<GraphStage> loading = graph->GetStage(0);
		loading->SetWeight(0.40f);
		loading->QueueJobs({ loadingJob });
		graph->GetStage(1)->SetWeight(0.60f);

        jobSystem.QueueGraph(graph);

		static float progress = 0.0f;

		Timer timer;
		while (!graph->HasFinished())
		{
			progress = graph->GetProgress();
			std::cout << fmt::format("{} progress: {}%", graph->GetName(), progress * 100.0f) << std::endl;

			std::this_thread::sleep_for(100ms);
		}

		std::cout << fmt::format("Total time {} ms", timer.ElapsedMillis()) << std::endl;

		jobSystem.Terminate();
	}
}
