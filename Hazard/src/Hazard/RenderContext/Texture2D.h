#pragma once

#include "Hazard/Assets/Asset.h"
#include "HazardRendererCore.h"

namespace Hazard
{
	//Texture contains texture atlas coords, selected from a source image
	class Texture2DAsset : public Asset 
	{
	public:
		Texture2DAsset(Ref<AssetPointer> sourceImage) : m_SourceImage2D(sourceImage) {};
		Ref<AssetPointer> GetSourceImageAsset() { return m_SourceImage2D; }

	private:
		Ref<AssetPointer> m_SourceImage2D = nullptr;
	};
}