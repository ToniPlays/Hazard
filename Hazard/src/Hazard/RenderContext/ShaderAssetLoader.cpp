
#include <hzrpch.h>
#include "ShaderAssetLoader.h"
#include "Core/GraphicsContext.h"
#include "Hazard/Assets/AssetPack.h"
#include "ShaderCompiler.h"

#include <Hazard/RenderContext/ShaderAsset.h>
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Core/Application.h"

namespace Hazard
{
	struct ShaderCodeResult
	{
		uint32_t Stage;
		HazardRenderer::RenderAPI Api;
		std::string Source;
	};

	Ref<JobGraph> ShaderAssetLoader::Load(AssetMetadata& metadata)
	{
		using namespace HazardRenderer;

		Ref<Job> job = Ref<Job>::Create(fmt::format("Shader load {}", metadata.Handle), LoadShader, metadata.Handle);

		Ref<JobGraph> graph = Ref<JobGraph>::Create(fmt::format("Shader load {}", metadata.Handle), 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}

	Ref<JobGraph> ShaderAssetLoader::Save(Ref<Asset>& asset)
	{
		return nullptr;
	}

	Ref<JobGraph> ShaderAssetLoader::DataFromSource(const std::filesystem::path& path)
	{
#ifdef HZR_SHADER_COMPILER
		using namespace HazardRenderer;

		auto sources = ShaderCompiler::GetShaderSources(path);
		if (sources.size() == 0) return nullptr;

		std::vector<RenderAPI> compileFor = { RenderAPI::OpenGL, RenderAPI::Vulkan, RenderAPI::Metal };
		std::vector<Ref<Job>> jobs;

		for (auto& api : compileFor)
		{
			for (auto& [stage, source] : sources)
			{
				Ref<Job> job = Ref<Job>::Create(fmt::format("Shader create {} ({})", File::GetName(path), stage), LoadSourceCode, source, stage, api);
				jobs.push_back(job);
			}
		}

		Ref<Job> assetJob = Ref<Job>::Create("Shader asset create", CreateShaderAsset, path);

		Ref<JobGraph> graph = Ref<JobGraph>::Create("Shader from source", 2);
		Ref<GraphStage> stage = graph->GetStage(0);
		Ref<GraphStage> assetStage = graph->GetStage(1);
		assetStage->QueueJobs({ assetJob });
		stage->QueueJobs(jobs);
        
		return graph;
#else
        return nullptr;
#endif
	}

	Ref<JobGraph> ShaderAssetLoader::Create(const std::filesystem::path& base, const std::filesystem::path& internalPath)
	{
		return nullptr;
	}

	Buffer ShaderAssetLoader::ToBinary(Ref<Asset> asset)
	{
		static std::mutex mtx;
		std::scoped_lock lock(mtx);

		using namespace HazardRenderer;
		ShaderAssetDataHeader header = {};
		Ref<ShaderAsset> shaderAsset = asset.As<ShaderAsset>();

		uint32_t stageCount = 0;

		for (auto& [api, shader] : shaderAsset->ShaderCode)
		{
			header.IncludedAPIs |= BIT(api);
			stageCount = shader.size();

			for (auto& [stage, code] : shader)
				header.Stages |= stage;
		}

		CachedBuffer buffer(1024 * 1024);
		buffer.Write(&header, sizeof(ShaderAssetDataHeader));

		std::vector<std::vector<std::string>> orderedSource(shaderAsset->ShaderCode.size());

		uint32_t apiIndex = 0;

		//Loop trough api sources
		for (uint32_t i = 0; i < 16; i++)
		{
			if (!(header.IncludedAPIs & BIT(i))) continue;

			std::vector<std::string> sourceStrings(stageCount);

			for (uint32_t s = 0; s < 32; s++)
			{
				uint32_t stageFlag = BIT(s);
				if (!(header.Stages & stageFlag)) continue;

				orderedSource[apiIndex].push_back(shaderAsset->ShaderCode[(RenderAPI)i][stageFlag]);
			}
			apiIndex++;
		}

		for (auto& sources : orderedSource)
		{
			ShaderAPIDataHeader apiHeader = {};
			for (auto& source : sources)
				apiHeader.DataSize += source.length();

			buffer.Write(apiHeader);
			
			for (auto& source : sources)
				buffer.Write(source);
		}

		return Buffer::Copy(buffer.GetData(), buffer.GetCursor());
	}

	void ShaderAssetLoader::LoadShader(JobInfo& info, AssetHandle handle)
	{
		using namespace HazardRenderer;
		Ref<ShaderAsset> shaderAsset = Ref<ShaderAsset>::Create();

		Buffer data = AssetManager::GetAssetData(handle);
		if (!data.Data)
		{
			AssetMetadata& meta = AssetManager::GetMetadata(handle);
			throw JobException(fmt::format("Shader asset data {} not found", meta.Key));
		}

		CachedBuffer buffer(data.Data, data.Size);

		ShaderAssetDataHeader header = buffer.Read<ShaderAssetDataHeader>();

		for (uint32_t i = 0; i < 16; i++)
		{
			if (!(header.IncludedAPIs & BIT(i))) continue;
			auto& sources = shaderAsset->ShaderCode[(RenderAPI)i];
			
			ShaderAPIDataHeader apiHeader = buffer.Read<ShaderAPIDataHeader>();

			for (uint32_t i = 0; i < 32; i++)
			{
				if (!(header.Stages & BIT(i))) continue;
				sources[BIT(i)] = buffer.Read<std::string>();
			}
		}

		data.Release();
		info.Job->SetResult(shaderAsset);
	}
	void ShaderAssetLoader::LoadSourceCode(JobInfo& info, const std::string& source, uint32_t stage, HazardRenderer::RenderAPI api)
	{
#ifdef HZR_SHADER_COMPILER
		info.Job->SetResult<ShaderCodeResult>({ stage, api, ShaderCompiler::GetShaderFromSource(stage, source, api) });
#endif
	}
	void ShaderAssetLoader::CreateShaderAsset(JobInfo& info, const std::filesystem::path& path)
	{
		auto results = info.PreviousStage->GetJobResults<ShaderCodeResult>();

		Ref<ShaderAsset> asset = Ref<ShaderAsset>::Create();
		asset->m_Type = AssetType::Shader;

		for (auto& result : results)
			asset->ShaderCode[result.Api][result.Stage] = std::move(result.Source);

		AssetPackElement element = {};
		element.AddressableName = path.string();
		element.AssetPackHandle = 0;
		element.Handle = UID();
		element.Type = AssetType::Shader;
		element.Data = AssetManager::AssetToBinary(asset);

		info.Job->SetResult(element);
	}
}
