
#include <hzrpch.h>
#include "ShaderAssetLoader.h"
#include "Backend/Core/ShaderCompiler.h"
#include <Hazard/RenderContext/ShaderAsset.h>

#include "Backend/Core/GraphicsContext.h"
#include "Hazard/Assets/AssetPack.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Core/Application.h"

namespace Hazard
{
	LoadType ShaderAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags)
	{
		using namespace HazardRenderer;
		RenderAPI api = GraphicsContext::GetRenderAPI();
		Ref<ShaderAsset> shaderAsset = Ref<ShaderAsset>::Create();

		auto cachePath = ShaderCompiler::GetCachedFilePath(metadata.Path, api);
		if (File::Exists(cachePath))
		{
			CachedBuffer buffer = File::ReadBinaryFile(cachePath);
			AssetPackElement element = buffer.Read<AssetPackElement>();

			while (buffer.Available())
			{
				ShaderCode code = buffer.Read<ShaderCode>();
				Buffer shaderCode = buffer.Read<Buffer>(code.Length);

				shaderAsset->ShaderCode.push_back({ code.Stage, Buffer::Copy(shaderCode) });
			}

			shaderAsset->m_Handle = element.Handle;
			shaderAsset->m_Type = (AssetType)element.Type;
			asset = shaderAsset;
			return LoadType::Cache;
		}

		shaderAsset->ShaderCode = ShaderCompiler::GetShaderBinariesFromSource(metadata.Path, api);
		shaderAsset->m_Type = AssetType::Shader;

		asset = shaderAsset;

		return LoadType::Source;
	}
	Ref<JobGraph> ShaderAssetLoader::LoadAsync(AssetMetadata& metadata, uint32_t flags)
	{
		Ref<JobNode> loadingJob = Ref<JobNode>::Create();
		loadingJob->DebugName = metadata.Path.filename().string();
		loadingJob->Callback = ([path = metadata.Path, handle = metadata.Handle](JobNode& node)->size_t {
			using namespace HazardRenderer;

			RenderAPI api = GraphicsContext::GetRenderAPI();
			Ref<ShaderAsset> shaderAsset = Ref<ShaderAsset>::Create();

			auto cachePath = ShaderCompiler::GetCachedFilePath(path, api);
			if (File::Exists(cachePath))
			{
				CachedBuffer buffer = File::ReadBinaryFile(cachePath);
				AssetPackElement element = buffer.Read<AssetPackElement>();

				while (buffer.Available())
				{
					ShaderCode code = buffer.Read<ShaderCode>();
					Buffer shaderCode = buffer.Read<Buffer>(code.Length);

					shaderAsset->ShaderCode.push_back({ code.Stage, Buffer::Copy(shaderCode) });
				}

				shaderAsset->m_Handle = element.Handle;
				shaderAsset->m_Type = (AssetType)element.Type;
				node.CreateBuffer<Ref<ShaderAsset>>();
				*node.Value<Ref<ShaderAsset>>() = std::move(shaderAsset);
				return (size_t)LoadType::Cache;
			}

			shaderAsset->ShaderCode = ShaderCompiler::GetShaderBinariesFromSource(path, api);
			shaderAsset->m_Type = AssetType::Shader;

			*node.Value<Ref<ShaderAsset>>() = std::move(shaderAsset);
			return (size_t)LoadType::Source;
		});

		Ref<JobGraph> graph = Ref<JobGraph>::Create("ShaderLoad");
		graph->AsyncJob(loadingJob);
		return graph;
	}
	bool ShaderAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
	Ref<JobGraph> ShaderAssetLoader::SaveAsync(Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		return nullptr;
		/*
		Timer timer;
		using namespace HazardRenderer;

		auto& metadata = AssetManager::GetMetadata(asset->GetHandle());

		std::vector<JobPromise> promises;

		auto promise = Application::Get().SubmitJob("Shader", [metadata](JobSystem* system, Job* job) -> size_t {

			for (uint32_t api = (uint32_t)RenderAPI::First; api <= (uint32_t)RenderAPI::Last; api++)
			{
				JOB_PROGRESS(job, api, RenderAPI::Last);

				auto binaries = ShaderCompiler::GetShaderBinariesFromSource(metadata.Path, (RenderAPI)api);
				size_t assetSize = ShaderCompiler::GetBinaryLength(binaries);
				CachedBuffer dataBuffer(sizeof(AssetPackElement) + assetSize);

				AssetPackElement element = {};
				element.Type = (uint32_t)AssetType::Shader;
				element.Handle = metadata.Handle;
				element.AssetDataSize = assetSize;

				dataBuffer.Write(element);

				for (auto& [shaderStage, binary] : binaries)
				{
					ShaderCode code = { shaderStage, binary.Size };
					dataBuffer.Write(code);
					dataBuffer.Write(binary.Data, binary.Size);
				}

				auto cachePath = ShaderCompiler::GetCachedFilePath(metadata.Path, (RenderAPI)api);

				if (!File::DirectoryExists(cachePath.parent_path()))
					File::CreateDir(cachePath.parent_path());

				File::WriteBinaryFile(cachePath, dataBuffer.GetData(), dataBuffer.GetSize());

				for (auto& code : binaries)
					code.ShaderCode.Release();

			}
			return 0;
			});
			*/
	}
}