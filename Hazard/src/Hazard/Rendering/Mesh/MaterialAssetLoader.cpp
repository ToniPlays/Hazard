#include <hzrpch.h>
#include "MaterialAssetLoader.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/ShaderLibrary.h"
#include "Material.h"

#include "Buffer/CachedBuffer.h"

namespace Hazard
{
	Ref<JobGraph> MaterialAssetLoader::Load(AssetMetadata& metadata)
	{
		Ref<Job> createAssetJob = Ref<Job>::Create("Material create", CreateMaterialAsset, CreateSettings());

		JobGraphInfo info = {
			.Name = "Material load",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Load", 1.0f, { createAssetJob } } }
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> MaterialAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		Ref<Job> saveAssetJob = Ref<Job>::Create("Material save", SaveMaterialAsset, asset);

		JobGraphInfo info = {
			.Name = "Material save",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Save", 1.0f, { saveAssetJob } } }
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> MaterialAssetLoader::Create(const CreateAssetSettings& settings)
	{
		HZR_CORE_ASSERT(settings.Settings, "Material settings required, no defaults available");
		CreateSettings matSettings = *(CreateSettings*)settings.Settings;
		Ref<Job> createAssetJob = Ref<Job>::Create("Material create", CreateMaterialAsset, matSettings);

		JobGraphInfo info = {
			.Name = "Material create",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Create", 1.0f, { createAssetJob } } }
		};

		return Ref<JobGraph>::Create(info);
	}
	void MaterialAssetLoader::CreateMaterialAsset(JobInfo& info, const CreateSettings& settings)
	{
		Ref<Material> material = Ref<Material>::Create();
		material->SetPipeline(ShaderLibrary::GetPipeline("PBR_Static"));
		material->Set("Metalness", 1.0f);
		material->Set("Roughness", 0.0f);
		info.Job->SetResult(material);
	}
	void MaterialAssetLoader::SaveMaterialAsset(JobInfo& info, Ref<Material> material)
	{
		Ref<CachedBuffer> buffer = Ref<CachedBuffer>::Create();
		buffer->Allocate(sizeof(MaterialAssetHeader));

		info.Job->SetResult(buffer);
	}
}

