
#include <hzrpch.h>
#include "ShaderLibrary.h"

#include "Backend/Core/ShaderCompiler.h"
#include "Backend/Core/GraphicsContext.h"

#include "Hazard/Rendering/Vertices.h"
#include "Hazard/RenderContext/ShaderAsset.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	void ShaderLibrary::Init()
	{
		using namespace HazardRenderer;


		std::vector<JobPromise> promises;
		//Line shader
		{
			auto promise = AssetManager::GetAssetAsync<ShaderAsset>("res/Shaders/Debug/lineShader.glsl");
			auto waitPromise = promise.Then([](JobSystem* system, Job* job) -> size_t {

				BufferLayout layout = LineVertex::Layout();
				Job* dependency = system->GetJob(job->Dependency);
				Ref<ShaderAsset> asset = *dependency->Value<Ref<ShaderAsset>>();

				PipelineSpecification specs = {};
				specs.DebugName = "LineShader";
				specs.DrawType = DrawType::Line;
				specs.Usage = PipelineUsage::GraphicsBit;
				specs.CullMode = CullMode::None;
				specs.LineWidth = 3.0f;
				specs.ShaderCodeCount = asset->ShaderCode.size();
				specs.pShaderCode = asset->ShaderCode.data();
				specs.pBufferLayout = &layout;

				s_LoadedShaders["lineShader"] = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
				return 0;
				});
			promises.push_back(waitPromise);
		}
		//Quad shader
		{
			auto promise = AssetManager::GetAssetAsync<ShaderAsset>("res/Shaders/2D/standard.glsl");
			auto waitPromise = promise.Then([](JobSystem* system, Job* job) -> size_t {

				BufferLayout layout = QuadVertex::Layout();
				Ref<ShaderAsset> asset = *system->GetJob(job->Dependency)->Value<Ref<ShaderAsset>>();

				PipelineSpecification specs = {};
				specs.DebugName = "QuadPipeline";
				specs.DrawType = DrawType::Fill;
				specs.Usage = PipelineUsage::GraphicsBit;
				specs.CullMode = CullMode::BackFace;
				specs.ShaderCodeCount = asset->ShaderCode.size();
				specs.pShaderCode = asset->ShaderCode.data();
				specs.pBufferLayout = &layout;
				s_LoadedShaders["standard"] = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
				return 0;
				});
			promises.push_back(waitPromise);
		}
		//PBR shader
		{
			auto promise = AssetManager::GetAssetAsync<ShaderAsset>("res/Shaders/pbr_static.glsl");
			auto waitPromise = promise.Then([](JobSystem* system, Job* job) -> size_t {

				BufferLayout layout = Vertex3D::Layout();
				Ref<ShaderAsset> asset = *system->GetJob(job->Dependency)->Value<Ref<ShaderAsset>>();

				PipelineSpecification specs = {};
				specs.DebugName = "PBR_Static";
				specs.DrawType = DrawType::Fill;
				specs.Usage = PipelineUsage::GraphicsBit;
				specs.CullMode = CullMode::BackFace;
				specs.ShaderCodeCount = asset->ShaderCode.size();
				specs.pShaderCode = asset->ShaderCode.data();
				specs.pBufferLayout = &layout;

				s_LoadedShaders["pbr_static"] = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
				return 0;
				});
			promises.push_back(waitPromise);
		}
		//Skybox
		{
			auto promise = AssetManager::GetAssetAsync<ShaderAsset>("res/Shaders/skybox.glsl");
			auto waitPromise = promise.Then([](JobSystem* system, Job* job) -> size_t {
				Ref<ShaderAsset> asset = *system->GetJob(job->Dependency)->Value<Ref<ShaderAsset>>();

				PipelineSpecification specs = {};
				specs.DebugName = "Skybox";
				specs.DrawType = DrawType::Fill;
				specs.Usage = PipelineUsage::GraphicsBit;
				specs.CullMode = CullMode::None;
				specs.DepthOperator = DepthOp::LessOrEqual;
				specs.DepthWrite = false;
				specs.ShaderCodeCount = asset->ShaderCode.size();
				specs.pShaderCode = asset->ShaderCode.data();

				s_LoadedShaders["skybox"] = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
				return 0;
				});
			promises.push_back(waitPromise);
		}

		//Compute shaders
		//EquirectangularToCubemap
		{
			auto promise = AssetManager::GetAssetAsync<ShaderAsset>("res/Shaders/Compute/EquirectangularToCubeMap.glsl");
			auto waitPromise = promise.Then([](JobSystem* system, Job* job) -> size_t {
				Ref<ShaderAsset> asset = *system->GetJob(job->Dependency)->Value<Ref<ShaderAsset>>();

				PipelineSpecification specs = {};
				specs.DebugName = "EquirectangularToCubemap";
				specs.Usage = PipelineUsage::ComputeBit;
				specs.ShaderCodeCount = asset->ShaderCode.size();
				specs.pShaderCode = asset->ShaderCode.data();

				s_LoadedShaders["EquirectangularToCubeMap"] = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
				return 0;
				});
			promises.push_back(waitPromise);
		}
		//Environment irradiance
		{
			auto promise = AssetManager::GetAssetAsync<ShaderAsset>("res/Shaders/Compute/EnvironmentIrradiance.glsl");
			auto waitPromise = promise.Then([](JobSystem* system, Job* job) -> size_t {
				Ref<ShaderAsset> asset = *system->GetJob(job->Dependency)->Value<Ref<ShaderAsset>>();

				PipelineSpecification specs = {};
				specs.DebugName = "EnvironmentIrradiance";
				specs.Usage = PipelineUsage::ComputeBit;
				specs.ShaderCodeCount = asset->ShaderCode.size();
				specs.pShaderCode = asset->ShaderCode.data();

				s_LoadedShaders["EnvironmentIrradiance"] = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
				return 0;
				});
			promises.push_back(waitPromise);
		}

		for (auto& promise : promises)
			promise.Wait();
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