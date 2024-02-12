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
        
		Ref<HazardRenderer::CubemapTexture> GetCubemap() const { return m_EnvironmentMap; }

    private:

		Ref<HazardRenderer::CubemapTexture> m_EnvironmentMap;
		EnvironmentMapSpec m_Spec;
	};
}
