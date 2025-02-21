
#include <hzrpch.h>
#include "ShaderAssetLoader.h"
#include "Core/GraphicsContext.h"
#include "ShaderCompiler.h"
#include "CompileException.h"

#include <Hazard/RenderContext/ShaderAsset.h>
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Assets/AssetPack.h"
#include "Hazard/Core/Application.h"

#include "Hazard/Rendering/Vertices.h"

#include "Utility/Hooks.h"


namespace Hazard
{
	Ref<JobGraph> ShaderAssetLoader::Load(AssetMetadata& metadata, const LoadAssetSettings& settings)
	{
		using namespace HazardRenderer;

		Ref<Job> loadingJob = Job::Create(fmt::format("Shader load: {}", metadata.Handle), LoadShaderAsset, metadata.Handle);

		JobGraphInfo info = {
			.Name = "Shader load",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "File load", 1.0f, { loadingJob } } },
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> ShaderAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		Ref<Job> binaryJob = Job::Create(fmt::format("{}", settings.TargetPath.string()), GenerateShaderAssetBinary, asset);

		JobGraphInfo info = {
			.Name = "Shader save",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Generate", 1.0f, { binaryJob } } },
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> ShaderAssetLoader::Create(const CreateAssetSettings& settings)
	{
		Ref<Job> preprocessJob = Job::Create(fmt::format("Shader create: {0}", settings.SourcePath.string()), PreprocessShaderSourceCode, settings);
		Ref<Job> createJob = Job::Create(fmt::format("Shader create: {0}", settings.SourcePath.string()), CreateShaderAsset);


		JobGraphInfo info = {
			.Name = "Shader create",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Preprocess", 0.1f, { preprocessJob } },
						{ "Compile", 0.8f, { } },
						{ "Finalize", 0.1f, { createJob } }
					  }
		};

		return Ref<JobGraph>::Create(info);
	}



	Coroutine ShaderAssetLoader::PreprocessShaderSourceCode(JobInfo info, const CreateAssetSettings& settings)
	{
		using namespace HazardRenderer;
		std::unordered_map<uint32_t, std::string> sources;

		ShaderParseFileResult result;
		Ref<ShaderAsset> asset = Ref<ShaderAsset>::Create();

		if (File::GetFileExtension(settings.SourcePath) == ".shader")
		{
			HZR_CORE_INFO("Loading shader as shader file");
			result = ShaderCompiler::ParseShaderFile(settings.SourcePath);

			std::stringstream ss;

			for (auto& shader : result.Shaders)
				ss << shader;

			result.Shaders.clear();
			ProcessShaderAsset(asset, result);

			File::WriteFile("res/debug/" + File::GetNameNoExt(settings.SourcePath) + ".glsl", ss.str());
			sources = ShaderCompiler::GetShaders(ss.str(), settings.SourcePath);
		}
		else
		{
			HZR_CORE_ASSERT(false, "We dropped support, plz fix");
			sources = ShaderCompiler::GetShaderSources(settings.SourcePath);
		}


		DescriptorSetLayout defaultLayout = { { SHADER_STAGE_ALL_GRAPHICS, "u_Camera", 0, DESCRIPTOR_TYPE_UNIFORM_BUFFER },
											  { SHADER_STAGE_FRAGMENT_BIT, "u_RadianceMap", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE },
											  { SHADER_STAGE_FRAGMENT_BIT, "u_IrradianceMap", 2, DESCRIPTOR_TYPE_SAMPLER_CUBE },
											  { SHADER_STAGE_FRAGMENT_BIT, "u_BRDFLut", 3, DESCRIPTOR_TYPE_SAMPLER_2D }
		};

		asset->m_Spec.DebugName = result.Name;
		asset->m_Spec.Usage = PipelineUsage::GraphicsBit;
		asset->m_Spec.pBufferLayout = &asset->m_Layout;
		asset->m_Spec.SetLayouts = { defaultLayout };
		asset->m_Spec.PushConstants = result.Constants;

		for (auto& layout : result.Layouts)
			asset->m_Spec.SetLayouts.push_back(layout);

		std::vector<Ref<Job>> loadingJobs;

		for (uint32_t api = (uint32_t)RenderAPI::First; api <= (uint32_t)RenderAPI::Last; api++)
		{
			for (auto& [stage, source] : sources)
			{
				Ref<Job> job = Job::Create(fmt::format("Compile shader"), CompileShaderSourceCode, api, stage, asset);
				loadingJobs.push_back(job);
			}
		}

		if (loadingJobs.size() == 0)
			throw JobException(fmt::format("No shader sources specified for {}", result.Name));

		info.Result(sources);
		info.ContinueWith(loadingJobs);
		info.Current->Finish();

		co_return;
	}

	Coroutine ShaderAssetLoader::CompileShaderSourceCode(JobInfo info, uint32_t api, uint32_t stageFlags, Ref<ShaderAsset> asset)
	{
		using namespace HazardRenderer;

		auto sources = info.Graph->GetResults<std::unordered_map<uint32_t, std::string>>()[0];
		if (!sources.contains(stageFlags))
			throw JobException("No source code provided");

		try
		{
			auto& source = sources[stageFlags];
			if (source.empty())
				throw JobException("Shader source is empty");

			auto compiled = ShaderCompiler::GetShaderFromSource(stageFlags, source, (RenderAPI)api);

			ShaderCompileResult result = {
				.API = api,
				.Data = std::move(compiled),
				.Flags = stageFlags,
				.Asset = asset,
			};

			info.Result(result);
		}
		catch (CompileException e)
		{
			throw JobException(fmt::format("Compile error on {}: {}", ShaderStageFlagsToString(stageFlags), e.what()));
		}

		info.Current->Finish();
		co_return;
	}

	Coroutine ShaderAssetLoader::CreateShaderAsset(JobInfo info)
	{
		using namespace HazardRenderer;

		auto results = info.Graph->GetResults<ShaderCompileResult>();
		Ref<ShaderAsset> asset = results[0].Asset;

		std::unordered_map<HazardRenderer::RenderAPI, std::unordered_map<uint32_t, std::string>> code;

		for (auto& result : results)
			code[(RenderAPI)result.API][result.Flags] = result.Data;

		PipelineSpecification& specs = asset->m_Spec;
		specs.Shaders = code[GraphicsContext::GetRenderAPI()];

		asset->m_Pipeline = Pipeline::Create(&specs);
		info.Result(asset);
		co_return;
	}

	Coroutine ShaderAssetLoader::GenerateShaderAssetBinary(JobInfo info, Ref<ShaderAsset> asset)
	{
		using namespace HazardRenderer;
		/*
		auto& code = "";

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

		Ref<CachedBuffer> buf = Ref<CachedBuffer>::Create();
		buf->Allocate(headers.size() * sizeof(ShaderAPIHeader) + totalCodeSize);

		for (auto& header : headers)
		{
			buf->Write(header);
			buf->Write(code[(RenderAPI)header.ApiFlags][header.StageFlags]);
		}

		info.Result(buf);
		info.Current->Finish();
		*/
		co_return;
	}

	Coroutine ShaderAssetLoader::LoadShaderAsset(JobInfo info, AssetHandle handle)
	{
		using namespace HazardRenderer;

		AssetMetadata& metadata = AssetManager::GetMetadata(handle);
		Ref<CachedBuffer> buffer = File::ReadBinaryFile(metadata.FilePath);

		AssetPack pack = {};
		pack.FromBuffer(buffer);

		Ref<ShaderAsset> shader = Ref<ShaderAsset>::Create();

		while (pack.AssetData->Available())
		{
			ShaderAPIHeader header = pack.AssetData->Read<ShaderAPIHeader>();
			auto source = pack.AssetData->Read<std::string>();
			//shader->ShaderCode[(RenderAPI)header.ApiFlags][header.StageFlags] = source;
		}

		info.Result(shader);
		info.Current->Finish();
		co_return;
	}

	void ShaderAssetLoader::ProcessShaderAsset(Ref<ShaderAsset> asset, const ShaderParseFileResult& result)
	{
		using namespace HazardRenderer;

		if (result.Type == "Line")
		{
			asset->m_Layout = LineVertex::Layout();
			asset->m_Spec.Flags |= PIPELINE_PRIMITIVE_TOPOLOGY_LINE_LIST | PIPELINE_DRAW_LINE;
		}
		else if (result.Type == "2D/Quad")
		{
			asset->m_Layout = QuadVertex::Layout();
			asset->m_Spec.Flags |= PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | PIPELINE_DRAW_FILL;
		}
		else if (result.Type == "2D/Circle")
		{
			asset->m_Layout = CircleVertex::Layout();
			asset->m_Spec.Flags |= PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | PIPELINE_DRAW_FILL;
		}
		else if (result.Type == "3D/Lit")
		{
			asset->m_Layout = Vertex3D::Layout();
			asset->m_Spec.Flags |= PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | PIPELINE_DRAW_FILL;
		}
		else throw JobException(fmt::format("Unknown shader type: {}", result.Type));

		Hooks<std::string, void(Ref<ShaderAsset>, const std::string&)> hooks;

		hooks.AddHook("DepthWrite ", [](Ref<ShaderAsset> asset, const std::string& value) mutable {
			if (value == "True")
				asset->m_Spec.Flags |= PIPELINE_DEPTH_WRITE;
		});

		hooks.AddHook("Depth ", [](Ref<ShaderAsset> asset, const std::string& value) mutable {
			asset->m_Spec.DepthOperator = DepthOp::LessOrEqual;
			asset->m_Spec.Flags |= PIPELINE_DEPTH_TEST;
			});


		for (auto& [name, value] : result.PipelineState)
			hooks.Invoke(name, asset, value);
	}
}
