#include "RuntimeExporter.h"
#include "Hazard/ECS/Loader/WorldDeserializer.h"
#include "Core/HazardEditor.h"

#include "Platform/OS.h"

void RuntimeExporter::Export(const std::filesystem::path& target)
{
	m_BuildTarget = target;

	if (!Directory::Exists(m_BuildTarget))
		Directory::Create(m_BuildTarget);

	JobGraphInfo exportInfo = {
		.Name = "Export",
		.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
		.Stages = { { "Preprocess", 0.1f, GetDependencyJobs() },
					{ "Dispatch jobs", 0.0f, { Ref<Job>::Create("Dispatch", DispatchAssetJobs) } } 
				},
	};

	Ref<JobGraph> graph = Ref<JobGraph>::Create(exportInfo);

	JobPromise promise = Hazard::Application::Get().GetJobSystem().QueueGraph(graph);

	promise.Then([target = m_BuildTarget](JobGraph& graph) {
		OS::OpenDirectory(target);
	});

	HZR_WARN("Starting export to: {0}", m_BuildTarget.string());
}

 std::vector<Ref<Job>> RuntimeExporter::GetDependencyJobs()
{
	using namespace Hazard;
	std::vector<Ref<Job>> jobs;
	jobs.reserve(m_TargetWorlds.size());

	for (auto& target : m_TargetWorlds)
		jobs.push_back(Ref<Job>::Create(File::GetNameNoExt(target.SourceFile), GetAssetDependenciesJob, target.SourceFile));

	return jobs;
}

 void RuntimeExporter::GetAssetDependenciesJob(JobInfo& job, const std::filesystem::path& sourcePath)
 {
	 if (!File::Exists(sourcePath))
		 throw JobException(fmt::format("File does not exist: {}", sourcePath.string()));

	 std::string source = File::ReadFile(sourcePath);
	 Hazard::WorldDeserializer deserializer(File::GetName(sourcePath), source);
	 deserializer.AddProgressHandler([&job](uint64_t i, uint64_t count) {
		 job.Job->Progress((float)i / (float)count);
	 });

	 auto referencedAssets = deserializer.GetReferencedAssets();
	 job.Job->SetResult(referencedAssets);
 }

 void RuntimeExporter::DispatchAssetJobs(JobInfo& job)
 {
	 std::unordered_map<AssetHandle, uint32_t> handles;
	 auto results = job.ParentGraph->GetResults<std::unordered_map<Hazard::AssetMetadata, uint32_t>>();
	 for (auto& result : results)
	 {
		 for (auto& [metadata, count] : result)
			 handles[metadata.Handle] += count;
	 }
	 for (auto& [handle, count] : handles)
	 {
		 Hazard::AssetMetadata& metadata = Hazard::AssetManager::GetMetadata(handle);
		 HZR_WARN("Including asset: {}, found {} times ({})", File::GetName(metadata.SourceFile), count, handle);
	 }
 }
