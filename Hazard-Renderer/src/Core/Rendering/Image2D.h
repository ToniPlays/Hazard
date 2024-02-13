#pragma once
#include "Image.h"
#include "Core/RenderContextCreateInfo.h"

namespace HazardRenderer 
{
	struct Image2DCreateInfo 
	{
		std::string DebugName;
		ImageUsage Usage;
		ImageFormat Format;
		Extent Extent;
		uint32_t MaxMips = 1;
		Buffer Data;
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

		static Ref<Image2D> Create(Image2DCreateInfo* info);
	};
}
