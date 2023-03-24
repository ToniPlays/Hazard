
#include <hzrpch.h>
#include "Hazard/Core/Application.h"
#include "PipelineAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "Backend/Core/Pipeline/RenderPass.h"

#include "Backend/Core/GraphicsContext.h"
#include "Backend/Core/ShaderCompiler.h"
#include "ShaderAsset.h"

namespace Hazard
{
	using namespace HazardRenderer;

	Ref<JobGraph> PipelineAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();
		return nullptr;
	}
	Ref<JobGraph> PipelineAssetLoader::Save(Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		return nullptr;

		auto& metadata = AssetManager::GetMetadata(asset->GetHandle());

		auto pipeline = asset.As<AssetPointer>()->Value.As<Pipeline>();
		PipelineSpecification specs = pipeline->GetSpecifications();

		PipelineAssetHeader header = {};
		header.Usage = specs.Usage;
		header.DrawType = specs.DrawType;
		header.CullMode = specs.CullMode;
		header.LineWidth = specs.LineWidth;
		header.DepthTest = specs.DepthTest;
		header.DepthWrite = specs.DepthWrite;
		header.DepthOperator = specs.DepthOperator;
		header.UseShaderLayout = specs.UseShaderLayout;
		header.ElementCount = 0;
		header.StageCount = pipeline->GetShader()->GetShaderCode().size();
		header.ShaderHandle = 0;

		return nullptr;
	}
}