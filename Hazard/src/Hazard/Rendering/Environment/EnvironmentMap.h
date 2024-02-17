#pragma once

#include "Hazard/Assets/Asset.h"
#include "Core/Rendering/Cubemap.h"

namespace Hazard
{
	class Material;

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
        
		Ref<HazardRenderer::Cubemap> GetRadianceMap() const { return m_RadianceMap; }
		Ref<HazardRenderer::Cubemap> GetIrradianceMap() const { return m_IrradianceMap; }
		Ref<Material> GetMaterial() const { return m_Material; }

		void Invalidate();
		void GenerateIrradiance();

    private:

		Ref<HazardRenderer::Cubemap> m_RadianceMap;
		Ref<HazardRenderer::Cubemap> m_IrradianceMap;
		Ref<Material> m_Material;
		EnvironmentMapSpec m_Spec;
	};
}
