#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "Backend/Core/Pipeline/Pipeline.h"

namespace Hazard
{
	struct PipelineLayoutElement
	{
		HazardRenderer::ShaderDataType Type;
		uint32_t NameLength;
		const char* Name;
	};

	struct PipelineAssetHeader
	{
		HazardRenderer::PipelineUsage Usage;
		HazardRenderer::DrawType DrawType;
		HazardRenderer::CullMode CullMode;
		float LineWidth;
		bool DepthTest;
		bool DepthWrite;
		bool UseShaderLayout;
		HazardRenderer::DepthOp DepthOperator;
		uint32_t ElementCount;
		uint32_t StageCount;
		uint64_t ShaderHandle;
	};

	class PipelineAssetLoader : public IAssetLoader
	{
	public:
		PipelineAssetLoader() = default;
		~PipelineAssetLoader() = default;

		LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset);
		bool Save(Ref<Asset>& asset);
	};
}