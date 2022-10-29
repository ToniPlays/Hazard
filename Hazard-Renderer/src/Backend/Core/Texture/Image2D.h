#pragma once
#include "Image.h"

namespace HazardRenderer 
{
	struct Image2DCreateInfo 
	{
		std::string DebugName;
		uint32_t Width;
		uint32_t Height;
		Buffer Data;
		uint32_t Mips = 1;
		ImageUsage Usage = ImageUsage::None;
		ImageFormat Format = ImageFormat::None;
		bool ClearLocalBuffer = true;
		bool GenerateMips = false;
	};

	class Image2D : public Image
	{
	public:
		virtual ~Image2D() = default;

		virtual const std::string& GetDebugName() const = 0;
		virtual ImageFormat GetFormat() const = 0;
		static Ref<Image2D> Create(Image2DCreateInfo* info);
	};
}
