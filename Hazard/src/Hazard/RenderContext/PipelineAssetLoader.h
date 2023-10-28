#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "Core/Rendering/Pipeline.h"

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
		HazardRenderer::DepthOp DepthOperator;
		uint32_t ElementCount;
		uint64_t StageCount;
		uint64_t ShaderHandle;
		uint32_t Flags;
	};

	class PipelineAssetLoader : public IAssetLoader
	{
	public:
		PipelineAssetLoader() = default;
		~PipelineAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset>& asset) override;
		Ref<JobGraph> FromSourceFile(const std::filesystem::path& path) override { return nullptr; };
		Ref<JobGraph> Create(const std::filesystem::path& path) override;

		Buffer ToBinary(Ref<Asset> asset) override { return Buffer(); };
	};
}
