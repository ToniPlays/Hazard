#pragma once

#include "Hazard/Assets/Asset.h"
#include "HazardRendererCore.h"

namespace Hazard
{
	class Image2D : public Asset 
	{
	public:
		Image2D(HazardRenderer::Image2DCreateInfo* info) {
			m_Image = HazardRenderer::Image2D::Create(info);
		}
		~Image2D() {}
		Ref<HazardRenderer::Image2D> GetSourceImage() { return m_Image; }

	private:
		Ref<HazardRenderer::Image2D> m_Image;
	};
	
	class Texture2D : public Asset {
	public:
		Texture2D(Ref<Image2D> sourceImage) : m_SourceImage(sourceImage) {};
		Ref<HazardRenderer::Image2D> GetSourceImage() { return m_SourceImage; }

	private:
		Ref<Image2D> m_SourceImage;
	};
}