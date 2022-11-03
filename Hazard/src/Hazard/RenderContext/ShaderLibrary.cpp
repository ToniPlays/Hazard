
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
		RenderAPI api = GraphicsContext::GetRenderAPI();

		//Line shader
		{
			BufferLayout layout = LineVertex::Layout();
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Debug/lineShader.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "LineShader";
			specs.DrawType = DrawType::Line;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::None;
			specs.LineWidth = 3.0f;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();
			specs.pBufferLayout = &layout;

			m_LoadedShaders["lineShader"] = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
		}
		//Quad shader
		{
			BufferLayout layout = QuadVertex::Layout();
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/2D/standard.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "QuadPipeline";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::BackFace;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();
			specs.pBufferLayout = &layout;

			m_LoadedShaders["standard"] = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
		}
		//PBR shader
		{
			BufferLayout layout = Vertex3D::Layout();
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/pbr_static.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "PBR_Static";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::BackFace;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();
			specs.pBufferLayout = &layout;

			m_LoadedShaders["pbr_static"] = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
		}
		//Skybox
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/skybox.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "Skybox";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::None;
			specs.DepthOperator = DepthOp::LessOrEqual;
			specs.DepthWrite = false;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();

			m_LoadedShaders["skybox"] = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
		}

		//Compute shaders
		//EquirectangularToCubemap
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Compute/EquirectangularToCubeMap.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "EquirectangularToCubemap";
			specs.Usage = PipelineUsage::ComputeBit;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();

			m_LoadedShaders["EquirectangularToCubeMap"] = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
		}
		//Environment irradiance
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Compute/EnvironmentIrradiance.glsl");

			PipelineSpecification specs = {};
			specs.DebugName = "EnvironmentIrradiance";
			specs.Usage = PipelineUsage::ComputeBit;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();

			m_LoadedShaders["EnvironmentIrradiance"] = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);
		}
	}
	Ref<HazardRenderer::Pipeline> ShaderLibrary::GetPipeline(const std::string& name)
	{
		return GetPipelinePtr(name)->Value.As<HazardRenderer::Pipeline>();
	}
	Ref<AssetPointer> ShaderLibrary::GetPipelinePtr(const std::string& name)
	{
		return m_LoadedShaders[name];
	}
}