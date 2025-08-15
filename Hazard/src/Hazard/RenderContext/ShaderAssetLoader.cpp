
#include <hzrpch.h>
#include "ShaderAssetLoader.h"
#include "Core/GraphicsContext.h"
#include "ShaderCompiler.h"
#include "CompileException.h"

#include <Hazard/RenderContext/ShaderAsset.h>
#include <Hazard/RenderContext/ShaderParser/ShaderTokenizer.h>
#include <Hazard/RenderContext/ShaderParser/ShaderParser.h>
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

		if (metadata.PackFlags & ASSET_PACK_REFERENCES_FILE)
		{
			ShaderCreateSettings create = {
				.FromExisting = true,
			};

			CreateAssetSettings settings = {
				.Type = AssetType::Shader,
				.AccessPath = metadata.FilePath,
				.SourcePath = metadata.SourceFile,
				.Settings = &create
			};

			return Create(settings);
		}

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
		Ref<Job> job = Job::Lambda("Dummy", [](JobInfo info) -> Coroutine {
			info.Result(Ref<CachedBuffer>::Create());
			co_return;
			});

		if (settings.Flags & ASSET_MANAGER_COMBINE_ASSET)
			job = Job::Create(fmt::format("{}", settings.TargetPath.string()), GenerateShaderAssetBinary, asset);

		JobGraphInfo info = {
			.Name = "Shader save",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Generate", 1.0f, { job } } },
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

		Ref<ShaderAsset> asset = Ref<ShaderAsset>::Create();
		ShaderCreateSettings shaderSettings = {};

		if (settings.Settings)
			shaderSettings = *(ShaderCreateSettings*)settings.Settings;

		if (!shaderSettings.FromExisting)
		{
			File::WriteFile(settings.SourcePath, "");

			info.ContinueWith({ Job::Lambda("Generate", [asset](JobInfo info) -> Coroutine {
				ShaderCompileResult result = {
					.Asset = asset
				};
                
				info.Result(result);

				info.Graph->AddOnFinished([asset]() {
					//Save asset after creation, problems if you do it first
					AssetManager::SaveAsset(asset);
					});
				co_return;
				})
				});

			co_return;
		}
        
        std::unordered_map<uint32_t, std::string> sources;

		if (File::GetFileExtension(settings.SourcePath) == ".shader")
		{
			auto tokenizer = Shading::ShaderTokenizer(settings.SourcePath);
			tokenizer.Tokenize();

			auto& tokens = tokenizer.GetTokens();

			auto parser = Shading::ShaderParser(tokens);
			parser.Parse();
            auto result = parser.GetRootScope();
            
            result.Scopes["scope"].RequireValue<std::string>("Type", asset->m_Type);
            
            asset->m_Spec.DebugName = result.Values["Shader"];
            asset->m_Spec.Usage = PipelineUsage::GraphicsBit;
            asset->m_Spec.MaxRayDepth = result.Scopes["scope"].GetValueOrDefault<uint32_t>("RayDepth", 1);
            asset->m_Spec.Flags |= result.Scopes["scope"].GetValueOrDefault("DetphWrite", true) ? PIPELINE_DEPTH_WRITE : 0;
            
			ShaderCompiler compiler(settings.SourcePath);

			for (auto& [key, sourceBlock] : result.Scopes["scope"].Scopes)
			{
				uint32_t version;
				result.Scopes["scope"].RequireValue<uint32_t>("Version", version);
				sources[ShaderStageFlagsFromString(key)] = compiler.GenerateGLSLFromBlock(version, sourceBlock);
			}

            HZR_CORE_INFO("Got shader: {}", asset->m_Spec.DebugName);
		}
        
		std::vector<Ref<Job>> loadingJobs;

		for (uint32_t api = (uint32_t)RenderAPI::First; api <= (uint32_t)RenderAPI::Last; api++)
		{
			for (auto& [stage, source] : sources)
			{
				Ref<Job> job = Job::Create(fmt::format("Compile shader {}", File::GetNameNoExt(settings.SourcePath)), CompileShaderSourceCode, api, stage, asset);
				loadingJobs.push_back(job);
			}
		}

		if (loadingJobs.size() == 0)
			throw JobException(fmt::format("No shader sources specified for {}", File::GetNameNoExt(settings.AccessPath)));

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

			ShaderCompiler compiler("");
			std::string compiled = compiler.GetShaderFromSource(stageFlags, source, (RenderAPI)api);


			ShaderCompileResult result = {
				.API = api,
				.Data = compiled,
				.Flags = stageFlags,
				.Asset = asset,
			};

			info.Result(result);
		}
		catch (CompileException e)
		{
			throw JobException(fmt::format("{} Compile error on {}: {}", info.Current->GetName(), ShaderStageFlagsToString(stageFlags), e.what()));
		}

		info.Current->Finish();
		co_return;
	}

	Coroutine ShaderAssetLoader::CreateShaderAsset(JobInfo info)
	{
		using namespace HazardRenderer;

		auto results = info.Graph->GetResults<ShaderCompileResult>();
		Ref<ShaderAsset> asset = results[0].Asset;

		ProcessShaderAsset(asset, asset->m_Type);

		if (asset->m_Spec.Usage == PipelineUsage::None)
		{
			info.Result(asset);
			co_return;
		}

		for (auto& result : results)
			asset->m_ShaderSources[result.API][result.Flags] = result.Data;

		PipelineSpecification& specs = asset->m_Spec;
		specs.Shaders = asset->m_ShaderSources[(uint32_t)GraphicsContext::GetRenderAPI()];

		if (specs.Shaders.size() == 0)
			throw JobException(fmt::format("{}: No shader sources found", info.Current->GetName()));
		else
			asset->m_Pipeline = Pipeline::Create(&specs);

		asset->m_ShaderSources.clear();
		info.Result(asset);
		co_return;
	}

	Coroutine ShaderAssetLoader::GenerateShaderAssetBinary(JobInfo info, Ref<ShaderAsset> asset)
	{
		using namespace HazardRenderer;

		Ref<CachedBuffer> buf = Ref<CachedBuffer>::Create();
		buf->AllowResize(true);
		PipelineSpecification& spec = asset->m_Spec;

		buf->Write(asset->m_Type);
		buf->Write(spec.DebugName);
		buf->Write(spec.Usage);
		buf->Write(spec.MaxRayDepth);
		buf->Write(spec.Flags);
		buf->Write(spec.DepthOperator);
		buf->Write<uint16_t>(spec.PushConstants.size());

		for (auto& constant : spec.PushConstants)
		{
			buf->Write(constant.Name);
			buf->Write(constant.Type);
			buf->Write(constant.Offset);
			buf->Write(constant.Flags);
		}

		buf->Write<uint16_t>(spec.SetLayouts.size());

		uint8_t index = 0;
		for (auto& set : spec.SetLayouts)
		{
			buf->Write(index);
			buf->Write<uint16_t>(set.GetElementCount());

			for (auto& e : set.GetElements())
			{
				buf->Write(e.Name);
				buf->Write(e.Binding);
				buf->Write(e.Length);
				buf->Write(e.Type);
				buf->Write(e.Flags);
			}
			index++;
		}

		for (auto& [api, stages] : asset->GetShaderSources())
		{
			buf->Write(api);
			buf->Write<uint8_t>(stages.size());

			for (auto& [stage, source] : stages)
			{
				buf->Write(stage);
				buf->Write(source);
			}
		}

		info.Result(buf);
		co_return;
	}

	Coroutine ShaderAssetLoader::LoadShaderAsset(JobInfo info, AssetHandle handle)
	{
		using namespace HazardRenderer;

		AssetMetadata& metadata = AssetManager::GetMetadata(handle);
		Ref<CachedBuffer> buffer = File::ReadBinaryFile(metadata.FilePath);

		AssetPack pack = {};
		pack.FromBuffer(buffer);
		Ref<CachedBuffer> buf = pack.AssetData;

		HZR_CORE_ASSERT(!(pack.Flags & ASSET_PACK_REFERENCES_FILE), "We reference a file instead");

		Ref<ShaderAsset> shader = Ref<ShaderAsset>::Create();

		PipelineSpecification& spec = shader->m_Spec;

		shader->m_Type = buf->Read<std::string>();
		spec.DebugName = buf->Read<std::string>();
		spec.Usage = buf->Read<PipelineUsage>();
		spec.MaxRayDepth = buf->Read<uint16_t>();
		spec.Flags = buf->Read<uint32_t>();
		spec.DepthOperator = buf->Read<DepthOp>();

		uint32_t pushConstants = buf->Read<uint16_t>();

		for (uint32_t i = 0; i < pushConstants; i++)
		{
			auto& constant = spec.PushConstants.emplace_back();
			constant.Name = buf->Read<std::string>();
			constant.Type = buf->Read<ShaderDataType>();
			constant.Offset = buf->Read<uint32_t>();
			constant.Flags = buf->Read<uint32_t>();
		}

		uint32_t setCount = buf->Read<uint16_t>();

		for (uint32_t i = 0; i < setCount; i++)
		{
			uint32_t setIndex = buf->Read<uint8_t>();
			uint32_t elementCount = buf->Read<uint16_t>();
			auto& set = spec.SetLayouts.emplace_back();

			for (uint32_t e = 0; e < elementCount; e++)
			{
				auto& element = set.GetElements().emplace_back();
				element.Name = buf->Read<std::string>();
				element.Binding = buf->Read<uint32_t>();
				element.Length = buf->Read<uint32_t>();
				element.Type = buf->Read<DescriptorType>();
				element.Flags = buf->Read<uint32_t>();
			}
		}

		while (buf->Available())
		{
			uint32_t api = buf->Read<uint32_t>();
			uint8_t stages = buf->Read<uint8_t>();

			for (uint32_t i = 0; i < stages; i++)
			{
				uint32_t stage = buf->Read<uint32_t>();
				std::string source = buf->Read<std::string>();
				shader->m_ShaderSources[api][stage] = source;
			}
		}

		ProcessShaderAsset(shader, shader->m_Type);
		spec.pBufferLayout = &shader->m_Layout;
		spec.Shaders = shader->m_ShaderSources[(uint32_t)GraphicsContext::GetRenderAPI()];

		shader->m_Pipeline = Pipeline::Create(&spec);
		info.Result(shader);
		info.Current->Finish();
		co_return;
	}

	void ShaderAssetLoader::ProcessShaderAsset(Ref<ShaderAsset> asset, const std::string& result)
	{
		using namespace HazardRenderer;

		DescriptorSetLayout setLayout = { { SHADER_STAGE_ALL_GRAPHICS, "u_Camera", 0, DESCRIPTOR_TYPE_UNIFORM_BUFFER },
										  { SHADER_STAGE_FRAGMENT_BIT, "u_RadianceMap", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE },
										  { SHADER_STAGE_FRAGMENT_BIT, "u_IrradianceMap", 2, DESCRIPTOR_TYPE_SAMPLER_CUBE },
										  { SHADER_STAGE_FRAGMENT_BIT, "u_BRDFLut", 3, DESCRIPTOR_TYPE_SAMPLER_2D }
		};

		if (result == "Line")
		{
			asset->m_Layout = LineVertex::Layout();
			asset->m_Spec.Flags |= PIPELINE_PRIMITIVE_TOPOLOGY_LINE_LIST | PIPELINE_DRAW_LINE;
			asset->m_Spec.SetLayouts.push_back(setLayout);
		}
		else if (result == "2D/Quad")
		{
			asset->m_Layout = QuadVertex::Layout();
			asset->m_Spec.Flags |= PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | PIPELINE_DRAW_FILL;
		}
		else if (result == "2D/Circle")
		{
			asset->m_Layout = CircleVertex::Layout();
			asset->m_Spec.Flags |= PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | PIPELINE_DRAW_FILL;
		}
		else if (result == "3D/Lit")
		{
			asset->m_Layout = Vertex3D::Layout();
			asset->m_Spec.Flags |= PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | PIPELINE_DRAW_FILL;
		}
	}
}
