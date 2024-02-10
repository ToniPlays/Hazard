
#include <hzrpch.h>

#include "MeshAssetLoader.h"
#include "Importers/AssimpImporter.h"
#include "Hazard/Assets/AssetManager.h"

#include "Hazard/Core/Application.h"
#include "Mesh.h"

namespace Hazard
{

	Ref<JobGraph> MeshAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();
		return nullptr;
	}

	Ref<JobGraph> MeshAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		Ref<Job> saveJob = Ref<Job>::Create("Save", SaveMesh);

		JobGraphInfo info = {};
		info.Name = "Mesh save";
		info.Stages = { { "Generate", 1.0f, { saveJob } } };
		info.Flags = JOB_GRAPH_TERMINATE_ON_ERROR;

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> MeshAssetLoader::Create(const CreateAssetSettings& settings)
	{
		MeshCreationSettings importSettings;
		if (settings.Settings != nullptr)
			importSettings = *(MeshCreationSettings*)settings.Settings;


		Ref<AssimpImporter> importer = Ref<AssimpImporter>::Create(settings.SourcePath);
		Ref<Job> preprocessJob = Ref<Job>::Create(fmt::format("Mesh: {}", settings.SourcePath.string()), PreprocessDependencies, importer);
		Ref<Job> finalize = Ref<Job>::Create(fmt::format("Finalize Mesh: {}", settings.SourcePath.string()), FinalizeMesh, importer);

		JobGraphInfo info = {};
		info.Name = "Mesh create";
		info.Stages = { { "Preprocess", 0.1f, { preprocessJob } },
						{ "Dependency load", 0.8f, { } },
						{ "Finalize", 0.1f, { finalize } }
		};
		info.Flags = JOB_GRAPH_TERMINATE_ON_ERROR;

		return Ref<JobGraph>::Create(info);
	}
	void MeshAssetLoader::PreprocessDependencies(JobInfo& info, Ref<MeshImporter> importer)
	{
		auto textures = importer->GetTextures();
		auto meshes = importer->GetMeshes();
		auto animations = importer->GetAnimations();

		std::vector<Ref<Job>> generateJobs;

		for (uint32_t i = 0; i < meshes.size(); i++)
		{
			auto& mesh = meshes[i];
			Ref<Job> processMeshJob = Ref<Job>::Create(fmt::format("Node: {}", mesh.Name), ProcessMeshNode, importer, i);
			generateJobs.push_back(processMeshJob);
		}

		info.ParentGraph->ContinueWith(generateJobs);
	}
	void MeshAssetLoader::ProcessMeshNode(JobInfo& info, Ref<MeshImporter> importer, uint32_t meshIndex)
	{
		Job& jobRef = *info.Job;
		auto data = importer->GetMeshData(meshIndex, [&jobRef](uint32_t current, uint32_t total) mutable {
			jobRef.Progress((float)current / (float)total);
		});

		info.Job->SetResult(data);
	}
	void MeshAssetLoader::FinalizeMesh(JobInfo& info, Ref<MeshImporter> importer)
	{
		auto results = info.ParentGraph->GetResults<MeshImporter::MeshData>();

		Ref<Mesh> mesh = Ref<Mesh>::Create();
		mesh->GenerateMesh(results);

		info.Job->SetResult(mesh);
	}
	void MeshAssetLoader::SaveMesh(JobInfo& info)
	{
		HZR_CORE_INFO("SAVING THIS TINH");

		info.Job->SetResult(Buffer());
	}
}
