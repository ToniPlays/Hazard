
#include <hzrpch.h>
#include "ShaderLibrary.h"

#include "Core/ShaderCompiler.h"
#include "Core/GraphicsContext.h"

#include "Hazard/Rendering/Vertices.h"
#include "Hazard/RenderContext/ShaderAsset.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardRenderer;

	void ShaderLibrary::Init()
	{
		HZR_PROFILE_SCOPE();
		HZR_TIMED_FUNCTION();
		{
			BufferLayout layout = LineVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("LineShader.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "LineShader.glsl";
			specs.DrawType = DrawType::Line;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::None;
			specs.LineWidth = 3.0f;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();
			specs.pBufferLayout = &layout;

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["LineShader"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			BufferLayout layout = CircleVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("CircleShader.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "CircleShader.glsl";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::BackFace;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();
			specs.pBufferLayout = &layout;

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["CircleShader"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			BufferLayout layout = QuadVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("QuadShader.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "QuadShader.glsl";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::BackFace;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();
			specs.pBufferLayout = &layout;

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["QuadShader"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			BufferLayout layout = Vertex3D::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("PBR_Static.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "PBR_Static.glsl";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::BackFace;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();
			specs.pBufferLayout = &layout;

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["PBR_Static"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("Skybox.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Skybox.glsl";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::None;
			specs.DepthOperator = DepthOp::LessOrEqual;
			specs.DepthWrite = false;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["Skybox"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("EquirectangularToCubemap.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "EquirectangularToCubemap.glsl";
			specs.Usage = PipelineUsage::ComputeBit;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();

			Ref<AssetPointer> pointer = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
			s_LoadedPipelines["EquirectangularToCubemap"] = AssetManager::CreateMemoryOnly(AssetType::Pipeline, pointer);
		}
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("EnvironmentIrradiance.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "EnvironmentIrradiance.glsl";
			specs.Usage = PipelineUsage::ComputeBit;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();

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
