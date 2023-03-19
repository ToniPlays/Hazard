#pragma once

#include <iostream>
#include "JobSystem.h"

#include <spdlog/fmt/fmt.h>
#include <Profiling/Timer.h>
#include <Random.h>

namespace JobSystemTest
{
	static void AssetLoad(Ref<Job> job)
	{
		Random random;
		uint32_t count = 35;
		for (uint32_t i = 0; i < count; i++)
		{
			job->Progress((float)i / (float)count);
			std::this_thread::sleep_for(25ms);
		}

		//Initialize next stage jobs
		Ref<GraphStage> stage = job->GetJobGraph()->GetNextStage();
	}

	static void WorldLoad(Ref<Job> job)
	{
		Random random;
		uint32_t count = 100;
		for (uint32_t i = 0; i < count; i++)
		{
			job->Progress((float)i / (float)count);
			std::this_thread::sleep_for(25ms);
		}

		//Initialize next stage jobs
		Ref<GraphStage> stage = job->GetJobGraph()->GetNextStage();
		
		std::vector<Ref<Job>> jobs(64);

		for (uint32_t i = 0; i < jobs.size(); i++)
		{
			jobs[i] = Ref<Job>::Create(AssetLoad);
			jobs[i]->SetJobName(fmt::format("Asset load {}", i));
		}
		stage->QueueJobs(jobs);
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

		Ref<Job> loadingJob = Ref<Job>::Create(WorldLoad);
		loadingJob->SetJobName("World loading");

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
			if (progress == graph->GetProgress()) continue;
			progress = graph->GetProgress();
			std::cout << fmt::format("{} progress: {}%", "Some graph", graph->GetProgress() * 100.0f) << std::endl;

			continue;

			std::stringstream ss;
			for (const auto& thread : jobSystem.GetThreads())
			{
				ss << fmt::format("Thread {}: {}", thread->GetThreadID(), GetStatus(thread->GetStatus())) << "\n";
				if (thread->GetCurrentJob())
					ss << fmt::format(" - Job {}: {}% ", thread->GetCurrentJob()->GetName(), thread->GetCurrentJob()->GetProgress() * 100.0f) << "\n";
			}
			std::cout << ss.str() << std::endl;
		}
		graph->Wait();
		std::cout << fmt::format("Total time {} ms", timer.ElapsedMillis()) << std::endl;

		jobSystem.Terminate();
	}
}
