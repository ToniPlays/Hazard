#include "RuntimeExporter.h"
#include "Hazard/ECS/Loader/WorldAssetLoader.h"
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
					{ "Dispatch jobs", 0.0f, { Job::Create("Dispatch", DispatchAssetJobs, target / "Data" )}},
					{ "Asset generation", 0.75f, { } },
					{ "Finalize", 0.15f, { GetSaveWorldJobs() }},
				},
	};

	Ref<JobGraph> graph = Ref<JobGraph>::Create(exportInfo);

    //Promise<bool> promise = Hazard::Application::Get().GetJobSystem().QueueGraph(graph);

	//promise.Then([target = m_BuildTarget](JobGraph& graph) {
		//OS::OpenDirectory(target);
	//});

	HZR_WARN("Starting export to: {0}", m_BuildTarget.string());
}

std::vector<Ref<Job>> RuntimeExporter::GetDependencyJobs()
{
	using namespace Hazard;
	std::vector<Ref<Job>> jobs;
	jobs.reserve(m_TargetWorlds.size());

	for (auto& target : m_TargetWorlds)
		jobs.push_back(Job::Create(File::GetNameNoExt(target.SourceFile), GetAssetDependenciesJob, target.SourceFile));

	return jobs;
}

std::vector<Ref<Job>> RuntimeExporter::GetSaveWorldJobs()
{
	std::vector<Ref<Job>> jobs;
	jobs.reserve(m_TargetWorlds.size());

	for (auto& target : m_TargetWorlds)
	{
		Ref<Job> job = Job::Create("World save", SaveWorldAssetJob, m_BuildTarget, target.Handle);
		jobs.push_back(job);
	}

	return jobs;
}

void RuntimeExporter::GetAssetDependenciesJob(JobInfo& job, const std::filesystem::path& sourcePath)
{
	if (!File::Exists(sourcePath))
		throw JobException(fmt::format("File does not exist: {}", sourcePath.string()));

	std::string source = File::ReadFile(sourcePath);
	Hazard::WorldDeserializer deserializer(File::GetName(sourcePath), source);
	deserializer.AddProgressHandler([&job](uint64_t i, uint64_t count) {
		job.Current->Progress((float)i / (float)count);
	});


	auto referencedAssets = deserializer.GetReferencedAssets();
	//job.Job->SetResult(referencedAssets.Assets);

	for (auto& [metadata, messages] : referencedAssets.MissingAssets)
		for (auto& message : messages)
			HZR_WARN(message);
}

void RuntimeExporter::DispatchAssetJobs(JobInfo& job, std::filesystem::path& assetPath)
{
	using namespace Hazard;
	std::unordered_map<AssetHandle, uint32_t> handles;
	/*auto results = job.ParentGraph->GetResults<std::unordered_map<AssetMetadata, uint32_t>>();
	for (auto& result : results)
	{
		for (auto& [metadata, count] : result)
			handles[metadata.Handle] += count;
	}
	
	std::vector<Promise> promises;

	for (auto& [handle, count] : handles)
	{
		job.ParentGraph->Halt();

		AssetMetadata& metadata = AssetManager::GetMetadata(handle);
		SaveAssetSettings settings = {
			.TargetPath = assetPath / File::GetName(metadata.FilePath),
			.Flags = ASSET_MANAGER_COMBINE_ASSET,
		};

        Promise promise = AssetManager::GetAssetAsync(handle);
		promise.Then([settings, info = job](JobGraph& graph) {
            Promise savePromise = AssetManager::SaveAsset(graph.GetResult<Ref<Asset>>(), settings);
			savePromise.Then([info](JobGraph& graph) mutable {
				info.ParentGraph->Continue();
			});
		});
	}
    */
}

void RuntimeExporter::SaveWorldAssetJob(JobInfo& job, std::filesystem::path& assetPath, AssetHandle handle)
{
	using namespace Hazard;


	AssetMetadata& metadata = AssetManager::GetMetadata(handle);

	LoadAssetSettings loadSettings = {};
	loadSettings.Flags = ASSET_MANAGER_NO_DEPENENCY_LOADING;

	SaveAssetSettings settings = {
		.TargetPath = assetPath / File::GetName(metadata.FilePath),
		.Flags = ASSET_MANAGER_COMBINE_ASSET,
	};

    /*
	job.ParentGraph->Halt();
    Promise<Ref<Asset>> promise = AssetManager::GetAssetAsync(handle, loadSettings);
	promise.Then([settings, info = job](JobGraph& graph) {
        Promise savePromise = AssetManager::SaveAsset(graph.GetResult<Ref<Asset>>(), settings);
		savePromise.Then([info](JobGraph& graph) mutable {
			info.ParentGraph->Continue();
		});
	});
     */
}
