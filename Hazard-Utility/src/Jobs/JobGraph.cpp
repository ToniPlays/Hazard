#include "JobGraph.h"
#include "JobSystem.h"

#include "spdlog/fmt/fmt.h"

JobGraph::JobGraph(const JobGraphInfo& info)
{
	m_Info = info;

	for (auto& stage : m_Info.Stages)
	{
		for (uint32_t i = 0; i < stage.Jobs.size(); i++)
		{
			Ref<Job> job = stage.Jobs[i];
			job->m_JobGraph = this;
			job->m_InvocationId = i;
		}
	}
}

bool JobGraph::SubmitJobs(JobSystem* system)
{
	std::scoped_lock lock(m_JobMutex);
	m_JobSystem = system;
	auto& jobs = m_Info.Stages[m_StageIndex].Jobs;
	if (jobs.size() == 0) return false;

	system->QueueJobs(jobs);
	m_RunningJobs = (uint32_t)jobs.size();
	m_RunningJobs.notify_all();
	return true;
}

void JobGraph::ContinueWith(const std::vector<Ref<Job>>& jobs)
{
	auto& j = m_Info.Stages[m_StageIndex + 1].Jobs;
	uint32_t offset = (uint32_t)j.size();
	j.reserve(j.size() + jobs.size());

	for (uint32_t i = 0; i < jobs.size(); i++)
	{
		Ref<Job> job = jobs[i];
		job->m_JobGraph = this;
		job->m_InvocationId = offset + i;
		j.emplace_back(job);
	}
}

float JobGraph::GetProgress()
{
	return 0.0f;
}

void JobGraph::OnJobFinished(Ref<Job> job)
{
	{
		std::scoped_lock lock(m_JobMutex);
		m_RunningJobs--;
		m_RunningJobs.notify_all();

		if (m_RunningJobs > 0) return;

		if (m_StageIndex + 1 >= m_Info.Stages.size())
		{
			m_StageIndex++;
            m_JobSystem->OnGraphFinished(this);
            m_OnFinishedCallback.Invoke();
			m_HasFinished = true;
			m_HasFinished.notify_all();
			return;
		}
	}

	SubmitNextStage();
}
void JobGraph::OnJobFailed(Ref<Job> job)
{
	m_RunningJobs--;
	m_Failed = true;
	m_Failed.notify_all();

	auto exception = job->GetException();
	m_OnFailedCallback.Invoke(exception.value());

	if (m_Info.Flags & JOB_GRAPH_TERMINATE_ON_ERROR)
	{
        m_JobSystem->OnGraphFinished(this);
        m_OnFinishedCallback.Invoke();
		m_HasFinished = true;
		m_HasFinished.notify_all();

		m_JobSystem->TerminateGraphJobs(this);
	}
}

void JobGraph::SubmitNextStage()
{
	if (m_HasFinished) return;

	m_StageIndex++;
	if (!SubmitJobs(m_JobSystem))
	{
        m_JobSystem->OnGraphFinished(this);
		m_HasFinished = true;
		m_HasFinished.notify_all();
	}
}
