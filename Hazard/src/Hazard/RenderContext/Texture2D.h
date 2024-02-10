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
		Texture2DAsset() = default;

		~Texture2DAsset() = default;
		AssetType GetType() const override { return AssetType::Image; }

		uint32_t GetWidth() const { return m_SourceImage2D->GetExtent().Width; }
		uint32_t GetHeight() const { return m_SourceImage2D->GetExtent().Height; }

		void SetExtent(const HazardRenderer::Extent& extent);
		void SetMaxResolution(uint32_t resolution);
		void SetMaxMipLevels(uint32_t maxMips);
		void Invalidate(Buffer imageData);

		Ref<HazardRenderer::Image2D> GetSourceImage() const { return m_SourceImage2D; }
		Ref<HazardRenderer::Sampler> GetSampler() const { return m_ImageSampler; }

	private:

		HazardRenderer::Image2DCreateInfo m_ImageInfo;

		Ref<HazardRenderer::Image2D> m_SourceImage2D = nullptr;
		Ref<HazardRenderer::Sampler> m_ImageSampler = nullptr;
	};
}
