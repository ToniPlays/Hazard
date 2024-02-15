
#include <hzrpch.h>
#include "WorldAssetLoader.h"
#include "WorldDeserializer.h"
#include "WorldSerializer.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{
	Ref<JobGraph> WorldAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();

		Ref<Job> preprocessJob = Ref<Job>::Create(fmt::format("Preprocess: {}", metadata.Handle), PreprocessWorldFile, metadata.Handle);
		Ref<Job> finalizeJob = Ref<Job>::Create(fmt::format("Finalize world: {}", metadata.Handle), FinalizeWorld, metadata.Handle);

		JobGraphInfo pipeline = {
			.Name = "World load",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Preprocess", 0.1f, { preprocessJob } },
						{ "Asset load", 0.8f, { } },
						{ "Finalize",   0.1f, { finalizeJob } },
			}
		};

		return Ref<JobGraph>::Create(pipeline);
	}
	Ref<JobGraph> WorldAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		Ref<World> world = asset.As<World>();
		WorldSerializer serializer(world);

		Ref<Job> contentJob = Ref<Job>::Create("GetWorldContent", GetWorldContent, serializer, settings.Flags);

		JobGraphInfo pipeline = {
			.Name = "World save",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = {	{ "Processing", 1.0f, { contentJob } } }
		};

		return Ref<JobGraph>::Create(pipeline);
	}
	Ref<JobGraph> WorldAssetLoader::Create(const CreateAssetSettings& settings)
	{
		auto& file = settings.SourcePath;

		Ref<Job> createJob = Ref<Job>::Create("GetWorldContent", CreateWorld, file);

		JobGraphInfo pipeline = {
			.Name = "World create",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = {	{ "Create", 0.8f, { createJob } } }
		};

		return Ref<JobGraph>::Create(pipeline);
	}
	void WorldAssetLoader::GetWorldContent(JobInfo& info, WorldSerializer serializer, uint32_t assetSaveFlags)
	{
		std::string result = serializer.Serialize();
		Ref<CachedBuffer> buffer = Ref<CachedBuffer>::Create(Buffer::Copy(result.c_str(), result.length()));
		info.Job->SetResult(buffer);
	}
	void WorldAssetLoader::PreprocessWorldFile(JobInfo& info, AssetHandle handle)
	{
		AssetMetadata& metadata = AssetManager::GetMetadata(handle);
		std::string source;

		if (!metadata.SourceFile.empty())
			source = File::ReadFile(metadata.SourceFile);

		WorldDeserializer deserializer(source);

		deserializer.AddProgressHandler([job = info.Job](uint64_t index, uint64_t count) mutable {
			job->Progress(((float)index / (float)count) * 0.1f);
		});

		auto assets = deserializer.GetReferencedAssets();

		std::vector<Ref<Job>> assetJobs;
		assetJobs.reserve(assets.size());

		for (auto& [handle, count] : assets)
			assetJobs.push_back(Ref<Job>::Create(fmt::format("AssetLoad: {0}", handle), LoadRequiredAsset, handle));

		info.ParentGraph->ContinueWith(assetJobs);
	}
	void WorldAssetLoader::LoadRequiredAsset(JobInfo& info, AssetHandle handle)
	{
		AssetMetadata& metadata = AssetManager::GetMetadata(handle);
		if (!metadata.IsValid()) return;
		
		Ref<JobGraph> loadGraph = AssetManager::GetLoadGraph(metadata);
		JobPromise promise = info.ParentGraph->SubGraph(loadGraph);

		promise.Then([info](JobGraph&) mutable {
			info.ParentGraph->Continue();
		});

		info.ParentGraph->Halt();
	}
	void WorldAssetLoader::FinalizeWorld(JobInfo& info, AssetHandle handle)
	{
		AssetMetadata& metadata = AssetManager::GetMetadata(handle);
		std::string source;

		if (!metadata.SourceFile.empty())
			source = File::ReadFile(metadata.SourceFile);

		WorldDeserializer deserializer(source);

		Ref<World> world = deserializer.Deserialize();
		info.Job->SetResult(world);
	}
	void WorldAssetLoader::CreateWorld(JobInfo& info, const std::filesystem::path& file)
	{
		Ref<World> world = Ref<World>::Create(file.string());
		info.Job->SetResult(world);
	}
}
