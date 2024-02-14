#pragma once

#include "Hazard/Assets/Asset.h"
#include "HazardRenderer.h"

namespace Hazard
{
	struct EnvironmentMapSpec
	{
		uint32_t Samples;
		uint32_t Resolution;
	};

	class EnvironmentMap : public Asset 
	{
		friend class EnvironmentAssetLoader;

	public:
		EnvironmentMap();
		~EnvironmentMap() = default;

		AssetType GetType() const override { return AssetType::EnvironmentMap; }
		EnvironmentMapSpec GetSpec() const { return m_Spec; }
        
		Ref<HazardRenderer::Cubemap> GetCubemap() const { return m_EnvironmentMap; }
		Ref<HazardRenderer::DescriptorSet> GetDescriptorSet() const { return m_DescriptorSet; }

    private:

		Ref<HazardRenderer::Cubemap> m_EnvironmentMap;
		Ref<HazardRenderer::DescriptorSet> m_DescriptorSet;
		EnvironmentMapSpec m_Spec;
	};
}
