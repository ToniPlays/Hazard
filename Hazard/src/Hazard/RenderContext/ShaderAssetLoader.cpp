
#include <hzrpch.h>
#include "ShaderAssetLoader.h"
#include "Core/GraphicsContext.h"
#include "ShaderCompiler.h"

#include <Hazard/RenderContext/ShaderAsset.h>
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Assets/AssetPack.h"
#include "Hazard/Core/Application.h"

#include "Hazard/RenderContext/ShaderCompiler.h"


namespace Hazard
{
	Ref<JobGraph> ShaderAssetLoader::Load(AssetMetadata& metadata)
	{
		using namespace HazardRenderer;

		Ref<Job> loadingJob = Ref<Job>::Create(fmt::format("Shader load: {}", metadata.Handle), LoadShaderAsset, metadata.Handle);

		JobGraphInfo info = {
			.Name = "Shader load",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "File load", 1.0f, { loadingJob } } },
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> ShaderAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		Ref<Job> binaryJob = Ref<Job>::Create(fmt::format("{}", settings.TargetPath.string()), GenerateShaderAssetBinary, asset);

		JobGraphInfo info = {
			.Name = "Shader save",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "", 1.0f, { binaryJob } } },
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> ShaderAssetLoader::Create(const CreateAssetSettings& settings)
	{
		auto& file = settings.SourcePath;

		Ref<Job> loadingJob = Ref<Job>::Create(fmt::format("ShaderLoad: {0}", file.string()), LoadShaderSource, file);
		Ref<Job> createJob = Ref<Job>::Create(fmt::format("Shader create: {0}", file.string()), CreateShaderAsset);

		JobGraphInfo info = {
			.Name = "Shader load",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Preprocess", 0.1f, { loadingJob } },
							{ "Compile", 0.8f, { } },
							{ "Validate", 0.1f, { createJob } }
			}
		};

		return Ref<JobGraph>::Create(info);
	}

	void ShaderAssetLoader::LoadShaderSource(JobInfo& info, const std::filesystem::path& path)
	{
		using namespace HazardRenderer;
		std::unordered_map<uint32_t, std::string> sources = ShaderCompiler::GetShaderSources(path);
		info.Job->SetResult(sources);

		std::vector<Ref<Job>> loadingJobs;

		for (uint32_t api = (uint32_t)RenderAPI::First; api <= (uint32_t)RenderAPI::Last; api++)
		{
			for (auto& [stage, source] : sources)
			{
				uint32_t flags = BIT(stage);
				Ref<Job> job = Ref<Job>::Create(fmt::format("{} {}", RenderAPIToString((RenderAPI)api), Utils::ShaderStageToString(flags)), CompileShaderSourceCode, api, stage);
				loadingJobs.push_back(job);
			}
		}
		info.ParentGraph->ContinueWith(loadingJobs);
	}
	void ShaderAssetLoader::CompileShaderSourceCode(JobInfo& info, uint32_t api, uint32_t stageFlags)
	{
		using namespace HazardRenderer;

		auto sources = info.ParentGraph->GetResult<std::unordered_map<uint32_t, std::string>>();
		if (!sources.contains(stageFlags))
			throw JobException("Shader stage load failed");

		try
		{
			auto& source = sources[stageFlags];
			auto compiled = ShaderCompiler::GetShaderFromSource(stageFlags, source, (RenderAPI)api);

			ShaderCompileResult result = {
				.API = api,
				.Data = std::move(compiled),
				.Flags = stageFlags
			};

			info.Job->SetResult(result);
		}
		catch (std::exception e)
		{
			throw JobException(e.what());
		}
	}
	void ShaderAssetLoader::CreateShaderAsset(JobInfo& info)
	{
		using namespace HazardRenderer;
		std::vector<ShaderCompileResult> results = info.ParentGraph->GetResults<ShaderCompileResult>();

		Ref<ShaderAsset> asset = Ref<ShaderAsset>::Create();

		for (auto& result : results)
			asset->ShaderCode[(RenderAPI)result.API][result.Flags] = result.Data;

		info.Job->SetResult(asset);
	}
	void ShaderAssetLoader::GenerateShaderAssetBinary(JobInfo& info, Ref<ShaderAsset> asset)
	{
		using namespace HazardRenderer;
		auto& code = asset->ShaderCode;

		std::vector<ShaderAPIHeader> headers;
		uint64_t totalCodeSize = 0;

		for (auto& [api, shaders] : code)
		{
			for (auto& [stageFlags, shaderCode] : shaders)
			{
				ShaderAPIHeader& header = headers.emplace_back();
				header.ApiFlags = (uint32_t)api;
				header.StageFlags = stageFlags;
				totalCodeSize += shaderCode.length() + sizeof(uint64_t);
			}
		}

		Buffer buffer;
		buffer.Allocate(headers.size() * sizeof(ShaderAPIHeader) + totalCodeSize);

		CachedBuffer buf(buffer.Data, buffer.Size);

		for (auto& header : headers)
		{
			buf.Write(header);
			buf.Write(code[(RenderAPI)header.ApiFlags][header.StageFlags]);
		}

		info.Job->SetResult(buffer);
	}
	void ShaderAssetLoader::LoadShaderAsset(JobInfo& info, AssetHandle handle)
	{
		using namespace HazardRenderer;

		AssetMetadata& metadata = AssetManager::GetMetadata(handle);
		CachedBuffer buffer = File::ReadBinaryFile(metadata.FilePath);

		AssetPack pack = {};
		pack.FromBuffer(buffer);

		Ref<ShaderAsset> shader = Ref<ShaderAsset>::Create();

		CachedBuffer dataBuffer(pack.AssetData.Data, pack.AssetData.Size);

		while (dataBuffer.Available())
		{
			ShaderAPIHeader header = dataBuffer.Read<ShaderAPIHeader>();
			auto source = dataBuffer.Read<std::string>();
			shader->ShaderCode[(RenderAPI)header.ApiFlags][header.StageFlags] = source;
		}

		info.Job->SetResult(shader);

		pack.AssetData.Release();
	}
}
