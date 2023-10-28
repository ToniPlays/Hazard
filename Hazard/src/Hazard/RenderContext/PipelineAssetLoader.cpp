
#include <hzrpch.h>
#include "Hazard/Core/Application.h"
#include "PipelineAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "Core/Rendering/RenderPass.h"

#include "Core/GraphicsContext.h"
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

		auto pipeline = asset.As<AssetPointer>()->Value.As<Pipeline>();
		PipelineSpecification specs = pipeline->GetSpecifications();

		PipelineAssetHeader header = {};
		header.Usage = specs.Usage;
		header.DepthOperator = specs.DepthOperator;
		header.ElementCount = 0;
		header.StageCount = pipeline->GetShader()->GetShaderCode().size();
		header.ShaderHandle = 0;
		header.Flags = specs.Flags;

		return nullptr;
	}

	Ref<JobGraph> PipelineAssetLoader::Create(const std::filesystem::path& path)
	{
		HZR_CORE_ASSERT(false, "");
		return Ref<JobGraph>();
	}
}
