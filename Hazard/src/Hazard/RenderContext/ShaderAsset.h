#pragma once

#include "Hazard/Assets/Asset.h"
#include "Buffer/Buffer.h"

#include "HazardRendererCore.h"

namespace Hazard
{
	class ShaderAsset : public Asset
	{
		friend class ShaderAssetLoader;

	public:
		ShaderAsset() = default;
		~ShaderAsset() = default;

		AssetType GetType() const override { return AssetType::Shader; }

		Ref<HazardRenderer::Pipeline> GetPipeline() const { return m_Pipeline; }

		void Invalidate();

	public:
		HazardRenderer::PipelineSpecification m_Spec;
		HazardRenderer::BufferLayout m_Layout = {};
		Ref<HazardRenderer::Pipeline> m_Pipeline;
	};
}