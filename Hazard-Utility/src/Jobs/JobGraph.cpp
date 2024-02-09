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

void JobGraph::Execute(JobSystem* system)
{
	m_JobSystem = system;
	auto& jobs = m_Info.Stages[m_CurrentStage].Jobs;
	if (jobs.size() == 0)
	{
		OnJobFinished(nullptr);
		return;
	}

	system->QueueJobs(jobs);
}

void JobGraph::Halt()
{
	if (m_IsHalted) return;

	m_IsHalted = true;
	m_IsHalted.notify_all();
	m_JobSystem->TerminateGraphJobs(this);

	std::string msg = fmt::format("Graph {} halted", GetName());
	m_JobSystem->m_MessageHook.Invoke(Severity::Warning, msg);
}

void JobGraph::Continue() 
{
	if (!m_IsHalted) return;

	m_IsHalted = false;
	m_IsHalted.notify_all();

	std::vector<Ref<Job>> remainingJobs;
	for (auto& job : GetCurrentStageInfo().Jobs)
	{
		if (job->GetStatus() == JobStatus::None)
			remainingJobs.push_back(job);
	}

	if (remainingJobs.size() == 0)
		SubmitNextStage();
	else 
		m_JobSystem->QueueJobs(remainingJobs);

	std::string msg = fmt::format("Graph {} continuing after halt", GetName());
	m_JobSystem->m_MessageHook.Invoke(Severity::Warning, msg);
}

void JobGraph::ContinueWith(const std::vector<Ref<Job>>& jobs)
{
	if (m_CurrentStage + 1 >= m_Info.Stages.size()) return;

	for (uint32_t i = 0; i < jobs.size(); i++)
	{
		Ref<Job> job = jobs[i];
		job->m_JobGraph = this;
		job->m_InvocationId = i;
	}

	std::scoped_lock lock(m_StageMutex);
	auto& stageJobs = m_Info.Stages[m_CurrentStage + 1].Jobs;
	stageJobs.insert(stageJobs.end(), jobs.begin(), jobs.end());
}

float JobGraph::GetProgress()
{
	float progress = 0.0;

	for (uint32_t i = 0; i < m_CurrentStage; i++)
	{
		const GraphStageInfo& info = m_Info.Stages[i];
		float stageProgress = 0.0f;

		for (auto& jobs : info.Jobs)
			stageProgress += jobs->GetProgress() / (float)info.Jobs.size();

		if (info.Jobs.size() == 0) stageProgress = 1.0f;

		progress += info.Weight * stageProgress;
	}

	return progress;
}

void JobGraph::OnJobFinished(Ref<Job> job)
{
	if (m_IsHalted) return;

	if (m_CurrentStage + 1 < m_Info.Stages.size())
	{
		SubmitNextStage();
		return;
	}

	if (m_HasFinished) return;

	m_CurrentStage++;
	m_HasFinished = true;
	m_Info.Flags |= JOB_GRAPH_SUCCEEDED;
	m_JobSystem->m_Hooks.Invoke(JobSystem::Finished, Ref<JobGraph>(this));

	try {
		m_OnCompleted.Invoke<JobGraph&>(*this);
	}
	catch (JobException e)
	{
		m_OnCompleted.Clear();
		std::string msg = fmt::format("Graph {} OnComplete error: {}", m_Info.Name, e.what());
		m_JobSystem->m_MessageHook.Invoke(Severity::Error, msg);
	}

	m_JobSystem->OnGraphFinished(Ref<JobGraph>(this));
	m_HasFinished.notify_all();
}
void JobGraph::OnJobFailed(Ref<Job> job)
{
	if (m_Info.Flags & JOB_GRAPH_TERMINATE_ON_ERROR)
	{
		m_JobSystem->TerminateGraphJobs(this);
		m_HasFinished = true;
		m_Info.Flags |= JOB_GRAPH_FAILED;

		m_JobSystem->m_Hooks.Invoke(JobSystem::Failure, this);
		m_CurrentStage = m_Info.Stages.size() - 1;

		try
		{
			m_OnCompleted.Invoke<JobGraph&>(*this);
		}
		catch (JobException e)
		{
			m_OnCompleted.Clear();
			std::string msg = fmt::format("Graph {} OnComplete error: {}", m_Info.Name, e.what());
			m_JobSystem->m_MessageHook.Invoke(Severity::Error, msg);
		}

		m_JobSystem->OnGraphFinished(this);
		m_HasFinished.notify_all();
		return;
	}

	if (m_IsHalted) return;

	if (m_CurrentStage + 1 < m_Info.Stages.size())
		SubmitNextStage();
}

void JobGraph::SubmitNextStage()
{
	for (auto& job : m_Info.Stages[m_CurrentStage].Jobs)
		if (job->GetStatus() != JobStatus::Success && job->GetStatus() != JobStatus::Failure)
			return;

	m_CurrentStage++;
	if (!m_JobSystem->QueueJobs(m_Info.Stages[m_CurrentStage].Jobs))
		OnJobFinished(nullptr);
}
