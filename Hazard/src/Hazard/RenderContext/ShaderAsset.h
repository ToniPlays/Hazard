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
		~ShaderAsset() {
			std::cout << "Destroyed: " << m_Spec.DebugName << std::endl;
		};

		AssetType GetType() const override { return AssetType::Shader; }

		const HazardRenderer::PipelineSpecification& GetSpecifications() const { return m_Spec; }
		Ref<HazardRenderer::Pipeline> GetPipeline() const { return m_Pipeline; }
		const std::string& GetShaderType() const { return m_Type; }

		const std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::string>>& GetShaderSources() const { return m_ShaderSources; }

		void Invalidate();

	public:
		std::string m_Type;
		HazardRenderer::PipelineSpecification m_Spec;
		HazardRenderer::BufferLayout m_Layout = {};
		Ref<HazardRenderer::Pipeline> m_Pipeline;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::string>> m_ShaderSources;
	};
}
