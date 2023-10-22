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
	JOB_FLAGS_ABORT_ON_FAILURE = BIT(1),
	JOB_FLAGS_SILENT_FAILURE = BIT(2)
};

enum class JobStatus
{
	None = 0,
	Failure = -1,
	Executing = 1,
	Success = 2
};
