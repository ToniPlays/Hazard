#pragma once

enum class ThreadStatus
{
	None,
	Terminated,
	Waiting,
	Executing
};

enum JobFlags
{
	JOB_FLAGS_SUCCEEDED = BIT(0),
	JOB_FLAGS_ABORT_ON_FAILURE = BIT(1)
};

enum class JobStatus
{
	Failure = -1,
	Executing = 0,
	Success = 1
};
