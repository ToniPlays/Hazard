#include <hzrpch.h>
#include "MaterialAssetLoader.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/ShaderLibrary.h"
#include "Material.h"
#include "Hazard/Assets/AssetPack.h"

#include "Buffer/CachedBuffer.h"
#include <Hazard/Rendering/RenderEngine.h>

namespace Hazard
{
	Ref<JobGraph> MaterialAssetLoader::Load(AssetMetadata& metadata, const LoadAssetSettings& settings)
	{
		Ref<Job> loadMaterialJob = Job::Create(fmt::format("Material {}", metadata.Handle), LoadMaterialAsset, metadata.FilePath);

		JobGraphInfo info = {
			.Name = "Material load",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Load", 1.0f, { loadMaterialJob } },
			},
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> MaterialAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		Ref<Job> saveAssetJob = Job::Create("Material save", SaveMaterialAsset, asset);

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
		Ref<Job> createAssetJob = Job::Create("Material create", CreateMaterialAsset, matSettings);

		JobGraphInfo info = {
			.Name = "Material create",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Create", 1.0f, { createAssetJob } } }
		};

		return Ref<JobGraph>::Create(info);
	}

	void MaterialAssetLoader::LoadMaterialAsset(JobInfo& info, const std::filesystem::path& path)
	{
		Ref<CachedBuffer> buffer = File::ReadBinaryFile(path);
		AssetPack pack = {};
		pack.FromBuffer(buffer);

		Buffer data(pack.AssetData->GetData(), pack.AssetData->GetSize());

		Ref<Material> material = Ref<Material>::Create(ShaderLibrary::GetPipeline("PBR_Static"));
		material->SetPushConstantData(data);

		Ref<Image2D> whiteTexture = Application::Get().GetModule<RenderContextManager>().GetWindow().GetContext()->GetDefaultResources().WhiteTexture;
		material->Set("u_Albedo", whiteTexture);

		//info.Job->SetResult(material);
	}

	void MaterialAssetLoader::CreateMaterialAsset(JobInfo& info, const CreateSettings& settings)
	{
		Ref<Material> material = Ref<Material>::Create();
		material->SetPipeline(ShaderLibrary::GetPipeline("PBR_Static"));

		Ref<Image2D> whiteTexture = Application::Get().GetModule<RenderContextManager>().GetWindow().GetContext()->GetDefaultResources().WhiteTexture;
		material->Set("u_Albedo", whiteTexture);

		//info.Job->SetResult(material);
	}

	void MaterialAssetLoader::SaveMaterialAsset(JobInfo& info, Ref<Material> material)
	{
		Ref<CachedBuffer> buffer = Ref<CachedBuffer>::Create();
		Buffer constants = material->GetPushConstantData();

		buffer->Allocate(constants.Size);
		buffer->Write(constants.Data, constants.Size);

		//info.Job->SetResult(buffer);
	}
}

