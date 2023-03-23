
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

	Ref<HazardRenderer::Pipeline> ShaderLibrary::GetPipeline(const std::string& name)
	{
		HZR_PROFILE_SCOPE();
		HZR_TIMED_FUNCTION();

		if (name == "LineShader")
		{
			BufferLayout layout = LineVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("Library/LineShader.glsl.hpack");

			PipelineSpecification specs = {};
			specs.DebugName = "LineShader.glsl";
			specs.DrawType = DrawType::Line;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::None;
			specs.LineWidth = 3.0f;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();
			specs.pBufferLayout = &layout;

			return AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline)->Value;
		}
		else if (name == "QuadShader")
		{
			BufferLayout layout = QuadVertex::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("Library/QuadShader.glsl.hpack");

			PipelineSpecification specs = {};
			specs.DebugName = "QuadShader.glsl";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::BackFace;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();
			specs.pBufferLayout = &layout;

			return AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline)->Value;
		}
		else if (name == "PBR_Static")
		{
			BufferLayout layout = Vertex3D::Layout();

			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("Library/PBR_Static.glsl.hpack");

			PipelineSpecification specs = {};
			specs.DebugName = "PBR_Static.glsl";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::BackFace;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();
			specs.pBufferLayout = &layout;

			return AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline)->Value;
		}
		else if (name == "Skybox")
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("Skybox.glsl.hpack_6214524764517733307");

			PipelineSpecification specs = {};
			specs.DebugName = "Skybox.glsl";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::None;
			specs.DepthOperator = DepthOp::LessOrEqual;
			specs.DepthWrite = false;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();

			return AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline)->Value;
		}
		else if (name == "EquirectangularToCubemap") 
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("EquirectangularToCubemap.glsl.hpack_12216603994862023646");

			PipelineSpecification specs = {};
			specs.DebugName = "EquirectangularToCubemap.glsl";
			specs.Usage = PipelineUsage::ComputeBit;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();

			return AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline)->Value;
		}
		else if (name == "EnvironmentIrradiance") 
		{
			Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>("EnvironmentIrradiance.glsl.hpack_6456767740621582699");

			PipelineSpecification specs = {};
			specs.DebugName = "EnvironmentIrradiance.glsl";
			specs.Usage = PipelineUsage::ComputeBit;
			specs.ShaderCodeCount = asset->ShaderCode.size();
			specs.pShaderCode = asset->ShaderCode.data();

			return AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline)->Value;
		}
		return nullptr;
	}
}
