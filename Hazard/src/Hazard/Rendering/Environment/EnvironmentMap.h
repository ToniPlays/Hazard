#pragma once

#include "Hazard/Assets/Asset.h"
#include "Hazard/RenderContext/Texture2D.h"

namespace Hazard
{
	class EnvironmentMap : public Asset 
	{
	public:
		EnvironmentMap() = default;

		Ref<Texture2DAsset> SourceImage = nullptr;
		Ref<AssetPointer> RadianceMap = nullptr;
		Ref<AssetPointer> IrradianceMap = nullptr;
		Ref<AssetPointer> PreFilterMap = nullptr;
		Ref<AssetPointer> BRDFLut = nullptr;

		static Ref<EnvironmentMap> Create(Ref<Texture2DAsset> sourceImage);
	};
}