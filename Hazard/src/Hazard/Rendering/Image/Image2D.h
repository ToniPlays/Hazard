#pragma once
#include "Hazard/Core/Core.h"
#include "Image.h"

namespace Hazard::Rendering 
{
	struct Image2DCreateInfo 
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Mips = 1;
		ImageUsage Usage = ImageUsage::None;
		ImageFormat Format = ImageFormat::None;
		bool ClearLocalBuffer = true;
	};

	class Image2D : public Image
	{
	public:
		virtual ~Image2D() = default;

		virtual ImageFormat GetFormat() const = 0;
		static Ref<Image2D> Create(Image2DCreateInfo* info);
	};
}
