#pragma once

#include <iostream>
#include <Jobs.h>

#include "Application/Test.h"

#include <spdlog/fmt/fmt.h>
#include <Profiling/Timer.h>
#include <Random.h>
#include <Jobs/JobSystem.h>

class JobGraphTest : public Test
{
public:
	void Reset() override;
	void Init() override;
	void Run() override;
	void Terminate() override;
    
    void InitializeHooks();
    
	const char* GetName() override { return "Job graph test"; }

private:
	Ref<JobGraph> m_Graph;
	Scope<JobSystem> m_JobSystem;
};

