#pragma once
#include "Hazard/Core/Core.h"
#include "Image.h"

namespace Hazard::Rendering 
{
	struct Image2DCreateInfo 
	{
		std::filesystem::path FilePath;
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Mips = 1;
		ImageWrap WrapMode = Repeat;
		TextureFilter* Filter = nullptr;
		ImageUsage Usage = ImageUsage::None;
		ImageFormat Format;
		void* Data = nullptr;
	};

	class Image2D : public Image
	{
	public:
		virtual ~Image2D() = default;

		static Ref<Image2D> Create(Image2DCreateInfo* info);
	};
}
