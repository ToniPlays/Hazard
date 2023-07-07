#pragma once

#include "Hazard/Assets/Asset.h"
#include "Hazard/RenderContext/Texture2D.h"

namespace Hazard
{
	class EnvironmentMap : public Asset 
	{
		friend class EnvironmentAssetLoader;
	public:
		EnvironmentMap();
		~EnvironmentMap();

        void Update(uint32_t samples, uint32_t resolution, Ref<Texture2DAsset> sourceImage);
        
		void GenerateRadiance();
		void GenerateIrradiance(Ref<AssetPointer> radianceMap);
		void GeneratePreFilter(Ref<AssetPointer> radianceMap);
        
        Ref<Texture2DAsset> GetSourceImage() const { return m_SourceImage; }

		Ref<AssetPointer> RadianceMap = nullptr;
		Ref<AssetPointer> IrradianceMap = nullptr;
		Ref<AssetPointer> PreFilterMap = nullptr;
		Ref<Texture2DAsset> BRDFLut = nullptr;
        
    private:
        Ref<Texture2DAsset> m_SourceImage;
        uint32_t m_Resolution;
        uint32_t m_Samples;
	};
}
