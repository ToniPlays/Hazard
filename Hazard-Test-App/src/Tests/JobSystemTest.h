#pragma once

#include <iostream>
#include <Jobs.h>

#include "Test.h"

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

	const char* GetName() { return "Job graph test"; }

private:
	JobSystem* m_JobSystem;
	Ref<JobGraph> m_Graph;
};

