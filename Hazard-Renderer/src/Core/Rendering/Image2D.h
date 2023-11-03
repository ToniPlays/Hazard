#pragma once
#include "Image.h"
#include "Core/RenderContextCreateInfo.h"

namespace HazardRenderer 
{
	struct Image2DCreateInfo 
	{
		std::string DebugName;
		Extent Extent;
		Buffer Data;
		uint32_t Mips = 1;
		ImageFormat Format;
		ImageUsage Usage;
	};

    struct ImageCopyRegion
    {
		Extent Extent;
        
        uint32_t X;
        uint32_t Y;
        uint32_t Z;

		void* Data;
		uint32_t DataSize;
    };

	class Image2D : public Image
	{
	public:
		virtual ~Image2D() = default;

		virtual const std::string& GetDebugName() const = 0;
		virtual ImageFormat GetFormat() const = 0;
        virtual Buffer ReadPixels(const ImageCopyRegion& region) = 0;

		static Ref<Image2D> Create(Image2DCreateInfo* info);
	};
}
