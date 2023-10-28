
#include <hzrpch.h>
#include "ShaderLibrary.h"

#include "Core/GraphicsContext.h"

#include "Hazard/Rendering/Vertices.h"
#include "Hazard/RenderContext/ShaderAsset.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardRenderer;

	void ShaderLibrary::Init(RenderAPI api)
	{
		HZR_PROFILE_SCOPE();
		HZR_TIMED_FUNCTION();
		{
			BufferLayout layout = LineVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("LineShader.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline LineShader";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.pBufferLayout = &layout;
			specs.Flags = PIPELINE_DRAW_LINE | PIPELINE_DEPTH_WRITE;
			specs.Shaders = asset->ShaderCode[api];

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["LineShader"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			BufferLayout layout = CircleVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("CircleShader.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline CircleShader";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.pBufferLayout = &layout;
			specs.Flags = PIPELINE_DRAW_FILL | PIPELINE_CULL_BACK_FACE | PIPELINE_DEPTH_WRITE;
			specs.Shaders = asset->ShaderCode[api];

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["CircleShader"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			BufferLayout layout = QuadVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("QuadShader.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline QuadShader";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.pBufferLayout = &layout;
			specs.Flags = PIPELINE_DRAW_FILL | PIPELINE_CULL_BACK_FACE | PIPELINE_DEPTH_WRITE;
			specs.Shaders = asset->ShaderCode[api];

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["QuadShader"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			BufferLayout layout = Vertex3D::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("PBR_Static.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline PBR_Static";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.pBufferLayout = &layout;
			specs.Flags = PIPELINE_DRAW_FILL | PIPELINE_CULL_BACK_FACE | PIPELINE_DEPTH_WRITE;
			specs.Shaders = asset->ShaderCode[api];

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["PBR_Static"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("Skybox.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline Skybox";
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.DepthOperator = DepthOp::LessOrEqual;
			specs.Flags = PIPELINE_DRAW_FILL;
			specs.Shaders = asset->ShaderCode[api];

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["Skybox"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("EquirectangularToCubemap.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline EquirectangularToCubemap";
			specs.Usage = PipelineUsage::ComputeBit;
			specs.Shaders = asset->ShaderCode[api];

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["EquirectangularToCubemap"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("EnvironmentIrradiance.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Pipeline EnvironmentIrradiance";
			specs.Usage = PipelineUsage::ComputeBit;
			specs.Shaders = asset->ShaderCode[api];

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["EnvironmentIrradiance"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
	}

	AssetHandle ShaderLibrary::GetPipelineAssetHandle(const std::string& name)
	{
		if (s_LoadedPipelines.find(name) == s_LoadedPipelines.end())
			return INVALID_ASSET_HANDLE;

		return s_LoadedPipelines[name];
	}
}
