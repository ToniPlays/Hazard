#pragma once

#include "Hazard/Assets/Asset.h"
#include "HazardRendererCore.h"

namespace Hazard
{
	class Texture2D : public Asset {
	public:
		Texture2D(const Ref<HazardRenderer::Image2D> sourceImage) : m_SourceImage(sourceImage) {};
		Ref<HazardRenderer::Image2D> GetSourceImage() { return m_SourceImage; }

	private:
		Ref<HazardRenderer::Image2D> m_SourceImage;
	};
}