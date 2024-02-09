#pragma once

#include "Hazard/Assets/Asset.h"
#include "Hazard/RenderContext/Texture2D.h"

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

        void Update(uint32_t samples, uint32_t resolution, AssetHandle sourceImage);
		void Invalidate();
        
		AssetHandle GetSourceImageHandle() const { return m_SourceImageHandle; }
		EnvironmentMapSpec GetSpec() const { return m_Spec; }

		void GenerateRadiance();
		void GenerateIrradiance(Ref<AssetPointer> radianceMap);
		void GeneratePreFilter(Ref<AssetPointer> radianceMap);
        
		Ref<AssetPointer> RadianceMap = nullptr;
		Ref<AssetPointer> IrradianceMap = nullptr;
		Ref<AssetPointer> PreFilterMap = nullptr;
		Ref<Texture2DAsset> BRDFLut = nullptr;
        
    private:
		AssetHandle m_SourceImageHandle;
		EnvironmentMapSpec m_Spec;
	};
}
