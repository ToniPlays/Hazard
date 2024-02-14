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
        
		Ref<HazardRenderer::Cubemap> GetCubemap() const { return m_EnvironmentMap; }
		Ref<Material> GetMaterial() const { return m_Material; }

		void Invalidate();

    private:

		Ref<HazardRenderer::Cubemap> m_EnvironmentMap;
		Ref<Material> m_Material;
		EnvironmentMapSpec m_Spec;
	};
}
