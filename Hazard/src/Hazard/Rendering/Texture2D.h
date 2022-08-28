#pragma once

#include "Hazard/Assets/Asset.h"
#include "HazardRendererCore.h"

namespace Hazard
{
	class Image2DAsset : public Asset 
	{
	public:
		Image2DAsset(HazardRenderer::Image2DCreateInfo* info) 
		{
			m_CoreImage = HazardRenderer::Image2D::Create(info);
		}
		~Image2DAsset() {}
		Ref<HazardRenderer::Image2D> GetCoreImage() { return m_CoreImage; }

	private:
		Ref<HazardRenderer::Image2D> m_CoreImage;
	};


	class Texture2DAsset : public Asset {
	public:
		Texture2DAsset(Ref<Image2DAsset> sourceImage) : m_SourceImage2D(sourceImage) {};
		Ref<Image2DAsset> GetSourceImageAsset() { return m_SourceImage2D; }

	private:
		Ref<Image2DAsset> m_SourceImage2D;
	};
}