
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
	static void LoadShader(Ref<Job> job, AssetHandle handle)
	{
		using namespace HazardRenderer;

		Ref<ShaderAsset> shaderAsset = Ref<ShaderAsset>::Create();
		Buffer buffer = AssetManager::GetAssetData(handle);
		CachedBuffer readBuffer(buffer.Data, buffer.Size);

		while (readBuffer.Available())
		{
			ShaderStageCode code = {};
			code.Stage = (ShaderStage)readBuffer.Read<uint32_t>();
			code.Size = readBuffer.Read<uint32_t>();
			code.ShaderCode = readBuffer.Read<Buffer>(code.Size);

			shaderAsset->ShaderCode.push_back(code);
		}
		job->GetStage()->SetResult(shaderAsset);

		AssetManager::GetMetadata(handle).LoadState = LoadState::Loaded;
	}

	Ref<JobGraph> ShaderAssetLoader::Load(AssetMetadata& metadata)
	{
		using namespace HazardRenderer;

		Ref<Job> job = Ref<Job>::Create(LoadShader, metadata.Handle);

		Ref<JobGraph> graph = Ref<JobGraph>::Create(fmt::format("Shader load {}", metadata.Handle), 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}
	Ref<JobGraph> ShaderAssetLoader::Save(Ref<Asset>& asset)
	{
		return nullptr;
	}
}
