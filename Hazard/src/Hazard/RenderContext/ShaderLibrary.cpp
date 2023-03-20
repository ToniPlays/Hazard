
#include <hzrpch.h>
#include "ShaderLibrary.h"

#include "Backend/Core/ShaderCompiler.h"
#include "Backend/Core/GraphicsContext.h"

#include "Hazard/Rendering/Vertices.h"
#include "Hazard/RenderContext/ShaderAsset.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardRenderer;

	static void LoadPipeline(Ref<Job> job, std::string file, PipelineSpecification spec, BufferLayout layout, std::unordered_map<std::string, Ref<AssetPointer>>* assets)
	{
		Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>(file);
		if (!asset) return;

		spec.ShaderCodeCount = asset->ShaderCode.size();
		spec.pShaderCode = asset->ShaderCode.data();
		spec.pBufferLayout = &layout;

		(*assets)[spec.DebugName] = AssetPointer::Create(Pipeline::Create(&spec), AssetType::Pipeline);
	}

	void ShaderLibrary::Init()
	{
		Timer timer;
		
		std::vector<Ref<Job>> jobs;
		{
			BufferLayout layout = LineVertex::Layout();
			std::string file = "res/Shaders/Debug/lineShader.glsl";

			PipelineSpecification specs = {};
			specs.DebugName = "LineShader";
			specs.DrawType = DrawType::Line;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::None;
			specs.LineWidth = 3.0f;

			Ref<Job> job = Ref<Job>::Create(LoadPipeline, file, specs, layout, &s_LoadedShaders);
			jobs.push_back(job);
		}
		{
			BufferLayout layout = QuadVertex::Layout();
			std::string file = "res/Shaders/2D/standard.glsl";

			PipelineSpecification specs = {};
			specs.DebugName = "QuadPipeline";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::BackFace;

			Ref<Job> job = Ref<Job>::Create(LoadPipeline, file, specs, layout, &s_LoadedShaders);
			jobs.push_back(job);
		}
		{
			BufferLayout layout = Vertex3D::Layout();
			std::string file = "res/Shaders/pbr_static.glsl";

			PipelineSpecification specs = {};
			specs.DebugName = "PBR_Static";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::BackFace;

			Ref<Job> job = Ref<Job>::Create(LoadPipeline, file, specs, layout, &s_LoadedShaders);
			jobs.push_back(job);
		}
		{
			std::string file = "res/Shaders/skybox.glsl";

			PipelineSpecification specs = {};
			specs.DebugName = "Skybox";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::None;
			specs.DepthOperator = DepthOp::LessOrEqual;
			specs.DepthWrite = false;

			Ref<Job> job = Ref<Job>::Create(LoadPipeline, file, specs, BufferLayout(), &s_LoadedShaders);
			jobs.push_back(job);
		}
		{
			std::string file = "res/Shaders/Compute/EquirectangularToCubeMap.glsl";

			PipelineSpecification specs = {};
			specs.DebugName = "EquirectangularToCubemap";
			specs.Usage = PipelineUsage::ComputeBit;

			Ref<Job> job = Ref<Job>::Create(LoadPipeline, file, specs, BufferLayout(), &s_LoadedShaders);
			jobs.push_back(job);
		}
		{
			std::string file = "res/Shaders/Compute/EnvironmentIrradiance.glsl";
			PipelineSpecification specs = {};
			specs.DebugName = "EnvironmentIrradiance";
			specs.Usage = PipelineUsage::ComputeBit;

			Ref<Job> job = Ref<Job>::Create(LoadPipeline, file, specs, BufferLayout(), &s_LoadedShaders);
			jobs.push_back(job);
		}

		Ref<JobGraph> shaderJobs = Ref<JobGraph>::Create("ShaderLibrary::Init", 1);
		shaderJobs->GetStage(0)->QueueJobs(jobs);
		auto& system = Application::Get().GetJobSystem();
		system.QueueGraph<bool>(shaderJobs);

		shaderJobs->Wait();

		HZR_CORE_INFO("Shader library initialized in {0} ms", timer.ElapsedMillis());
	}
	Ref<HazardRenderer::Pipeline> ShaderLibrary::GetPipeline(const std::string& name)
	{
		return GetPipelinePtr(name)->Value.As<HazardRenderer::Pipeline>();
	}
	Ref<AssetPointer> ShaderLibrary::GetPipelinePtr(const std::string& name)
	{
		if (s_LoadedShaders.find(name) == s_LoadedShaders.end()) return nullptr;
		return s_LoadedShaders[name];
	}
}
