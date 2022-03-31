#pragma once
#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN
#include "Image.h"

namespace HazardRenderer 
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
#endif