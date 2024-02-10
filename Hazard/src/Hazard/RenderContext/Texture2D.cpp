#include "Texture2D.h"
#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard
{
	void Texture2DAsset::SetExtent(const HazardRenderer::Extent& extent)
	{
		m_ImageInfo.Extent = extent;
	}

	void Texture2DAsset::SetMaxResolution(uint32_t resolution)
	{
		if (m_ImageInfo.Extent.Width > m_ImageInfo.Extent.Height)
		{
			float aspectRatio = (float)m_ImageInfo.Extent.Height / (float)m_ImageInfo.Extent.Width;
			m_ImageInfo.Extent.Width = resolution;
			m_ImageInfo.Extent.Height = (float)resolution * aspectRatio;
		}
		else
		{
			float aspectRatio = (float)m_ImageInfo.Extent.Width / (float)m_ImageInfo.Extent.Height;
			m_ImageInfo.Extent.Width = (float)resolution * aspectRatio;
			m_ImageInfo.Extent.Height = resolution;
		}
	}

	void Texture2DAsset::SetMaxMipLevels(uint32_t maxMips)
	{
		m_ImageInfo.MaxMips = maxMips;
	}

	void Texture2DAsset::Invalidate(Buffer imageData)
	{
		using namespace HazardRenderer;
		m_ImageInfo.DebugName = fmt::format("Image: {}", (uint64_t)this);
		m_ImageInfo.Format = ImageFormat::RGBA;
		m_ImageInfo.Usage = ImageUsage::Texture;
		m_ImageInfo.Data = imageData;

		m_SourceImage2D = Image2D::Create(&m_ImageInfo);
		m_ImageSampler = RenderEngine::GetResources().DefaultImageSampler;
	}
}
