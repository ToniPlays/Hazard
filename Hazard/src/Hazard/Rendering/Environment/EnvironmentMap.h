#pragma once

#include "Hazard/Assets/Asset.h"
#include "Hazard/RenderContext/Texture2D.h"

namespace Hazard
{
	class EnvironmentMap : public Asset 
	{
	public:
		EnvironmentMap();
		~EnvironmentMap()
		{
		}

		void GenerateRadiance(Ref<Texture2DAsset> sourceImage);
		void GenerateIrradiance(Ref<AssetPointer> radianceMap);
		void GeneratePreFilter(Ref<AssetPointer> radianceMap);

		Ref<Texture2DAsset> SourceImage = nullptr;
		Ref<AssetPointer> RadianceMap = nullptr;
		Ref<AssetPointer> IrradianceMap = nullptr;
		Ref<AssetPointer> PreFilterMap = nullptr;
		Ref<AssetPointer> BRDFLut = nullptr;

		static Ref<EnvironmentMap> Create(Ref<Texture2DAsset> sourceImage);
	};
}