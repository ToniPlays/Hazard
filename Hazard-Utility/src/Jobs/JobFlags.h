#pragma once

#include "UtilityCore.h"

enum class ThreadStatus
{
	None,
	Terminated,
	Waiting,
	Executing,
	Failed,
	Finished
};

enum JobGraphFlags
{
	JOB_GRAPH_TERMINATE_ON_ERROR = BIT(0),
	JOB_GRAPH_TERMINATED = BIT(1),
	JOB_GRAPH_FAILED = BIT(2),
	JOB_GRAPH_SUCCEEDED = BIT(3)
};

enum class JobStatus
{
	None = 0,
	Failure = -1,
	Executing = 1,
	Success = 2
};
