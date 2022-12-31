#pragma once

#include "Hazard/Assets/Asset.h"
#include "Hazard/RenderContext/Texture2D.h"

namespace Hazard
{
    struct EnvironmentMapCreateInfo
    {
        uint32_t Resolution;
        size_t Samples = 0;
        Ref<Texture2DAsset> SourceImage;
    };

	class EnvironmentMap : public Asset 
	{
	public:
		EnvironmentMap();
        
        void Update(uint32_t samples);
        
		void GenerateRadiance();
		void GenerateIrradiance(Ref<AssetPointer> radianceMap);
		void GeneratePreFilter(Ref<AssetPointer> radianceMap);
        
        Ref<Texture2DAsset> GetSourceImage() const { return m_SourceImage; }

		Ref<AssetPointer> RadianceMap = nullptr;
		Ref<AssetPointer> IrradianceMap = nullptr;
		Ref<AssetPointer> PreFilterMap = nullptr;
		Ref<Texture2DAsset> BRDFLut = nullptr;

		static Ref<EnvironmentMap> Create(EnvironmentMapCreateInfo* info);
        
    private:
        Ref<Texture2DAsset> m_SourceImage;
        uint32_t m_Resolution;
        uint32_t m_Samples;
	};
}
