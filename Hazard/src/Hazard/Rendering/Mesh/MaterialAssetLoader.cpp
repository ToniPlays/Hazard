#include <hzrpch.h>
#include "MaterialAssetLoader.h"

#include "Hazard/Assets/AssetManager.h"
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

	Coroutine MaterialAssetLoader::LoadMaterialAsset(JobInfo info, const std::filesystem::path& path)
	{
		Ref<CachedBuffer> buffer = File::ReadBinaryFile(path);
		AssetPack pack = {};
		pack.FromBuffer(buffer);

		uint32_t constantSize = pack.AssetData->Read<uint32_t>();

		Buffer data = pack.AssetData->Read<Buffer>(constantSize);

		Ref<Material> material = Ref<Material>::Create(AssetManager::AssetHandleFromFile("res/Shaders/PBR_Static.shader"));
		material->SetPushConstantData(data);

		uint32_t textureParamSize = pack.AssetData->Read<uint32_t>();

		Ref<Image2D> whiteTexture = Application::Get().GetModule<RenderContextManager>().GetWindow().GetContext()->GetDefaultResources().WhiteTexture;
		for (auto& [name, texture] : material->GetTextureParams())
			material->Set(name, whiteTexture);

		while (pack.AssetData->Available())
		{
			uint32_t binding = pack.AssetData->Read<uint32_t>();
			AssetHandle handle = pack.AssetData->Read<AssetHandle>();

			if (handle == INVALID_ASSET_HANDLE) continue;

			for (auto& [name, texture] : material->GetTextureParams())
			{
				if (texture.Binding == binding)
				{
					std::vector<Ref<Texture2DAsset>> asset = (co_await AssetManager::GetAssetAsync<Texture2DAsset>(handle));
					if (asset.size() == 0) break;

					material->Set(name, asset[0]);
					break;
				}
			}
		}

		info.Result(material);
		info.Current->Finish();
		co_return;
	}

	Coroutine MaterialAssetLoader::CreateMaterialAsset(JobInfo info, const CreateSettings& settings)
	{
		Ref<Material> material = Ref<Material>::Create();
		material->SetPipeline(INVALID_ASSET_HANDLE);

		Ref<Image2D> whiteTexture = Application::Get().GetModule<RenderContextManager>().GetWindow().GetContext()->GetDefaultResources().WhiteTexture;
		for (auto& [name, texture] : material->GetTextureParams())
			material->Set(name, whiteTexture);

		info.Result(material);
		info.Current->Finish();
		co_return;
	}

	Coroutine MaterialAssetLoader::SaveMaterialAsset(JobInfo info, Ref<Material> material)
	{
		Ref<CachedBuffer> buffer = Ref<CachedBuffer>::Create();
		Buffer constants = material->GetPushConstantData();

		uint32_t constantSize = constants.Size;
		uint32_t textureSize = material->GetTextureParams().size() * (sizeof(uint32_t) + sizeof(AssetHandle));

		buffer->Allocate(sizeof(uint32_t) * 2 + constantSize + textureSize);

		buffer->Write(constantSize);
		buffer->Write(constants.Data, constants.Size);
		buffer->Write(textureSize);

		for (auto& [name, texture] : material->GetTextureParams())
		{
			AssetHandle handle = texture.Handle;

			buffer->Write(texture.Binding);
			buffer->Write(handle);
		}

		info.Result(buffer);
		info.Current->Finish();
		co_return;
	}
}

