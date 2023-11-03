#pragma once

#include "Hazard/Assets/Asset.h"
#include "HazardRendererCore.h"

namespace Hazard
{
	//Texture contains texture atlas coords, selected from a source image
	class Texture2DAsset : public Asset
	{
		friend class ImageAssetLoader;

	public:
		Texture2DAsset(Ref<AssetPointer> sourceImage, Ref<HazardRenderer::Sampler> sampler) 
			: m_SourceImage2D(sourceImage), m_ImageSampler(sampler) {};

		~Texture2DAsset() {}
		Ref<AssetPointer> GetSourceImageAsset() const { return m_SourceImage2D; }
		Ref<HazardRenderer::Sampler> GetSampler() const { return m_ImageSampler; }

	private:
		Ref<AssetPointer> m_SourceImage2D = nullptr;
		Ref<HazardRenderer::Sampler> m_ImageSampler = nullptr;
	};
}
