#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "Backend/Core/Pipeline.h"

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
		size_t StageCount;
		size_t ShaderHandle;
	};

	class PipelineAssetLoader : public IAssetLoader
	{
	public:
		PipelineAssetLoader() = default;
		~PipelineAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset>& asset) override;
	};
}
