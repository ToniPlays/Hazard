
#include <hzrpch.h>
#include "TextureFactory.h"
#include "File.h"

#include "vendor/stb_image.h"
#include "Buffer/CachedBuffer.h"

namespace Hazard
{
	struct FileHeader
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Channels;
		uint64_t DataSize;
	};
	uint32_t TextureFactory::PixelSize(const HazardRenderer::ImageFormat& format)
	{
		using namespace HazardRenderer;
		switch (format)
		{
		case ImageFormat::None:				return 0;
		case ImageFormat::RED32F:			return 3;
		case ImageFormat::RGB:				return 3;
		case ImageFormat::RGBA:				return 4;
		case ImageFormat::RGBA16F:			return 4;
		case ImageFormat::RGBA32F:			return 4;
		case ImageFormat::RG16F:			return 2;
		case ImageFormat::RG32F:			return 2;
		case ImageFormat::SRGB:				return 2;
		case ImageFormat::DEPTH32F:			return 1;
		case ImageFormat::DEPTH24STENCIL8:	return 1;
		case ImageFormat::RED32I:           return 1;
		}
		return 0;
	}

	std::filesystem::path TextureFactory::GetCacheFile(const AssetHandle& handle)
	{
		return s_CacheDirectory / (std::to_string(handle) + ".hzrche");
	}

	CacheStatus TextureFactory::CacheStatus(const AssetHandle& handle)
	{
		return File::Exists(GetCacheFile(handle)) ? CacheStatus::Exists : CacheStatus::None;
	}

	TextureHeader TextureFactory::LoadTextureFromSourceFile(const std::filesystem::path& path, bool verticalFlip)
	{
		HZR_PROFILE_FUNCTION();
		HZR_CORE_ASSERT(File::Exists(path), "Source file does not exist");
		TextureHeader header = {};

		int w, h, channels;
		constexpr int desired = 4;
		{
			stbi_set_flip_vertically_on_load(verticalFlip);

			stbi_uc* data = stbi_load(path.string().c_str(), &w, &h, &channels, desired);
			HZR_CORE_ASSERT(data, "Data not loaded correctly");
			header.ImageData = Buffer(data, w * h * desired);
		}

		header.Width = w;
		header.Height = h;
		header.Channels = desired;

		//std::cout << "Image load from source " << path << " took " << timer.ElapsedMillis() << " ms" << std::endl;

		return header;
	}
}
