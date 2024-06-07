
#include <hzrpch.h>
#include "TextureFactory.h"
#include "File.h"

#include "vendor/stb_image.h"
#include "Buffer/CachedBuffer.h"

namespace Hazard
{
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
			case ImageFormat::RGB16F:           return 4;
			case ImageFormat::RGB32F:           return 4;
		}
		return 0;
	}

	TextureHeader TextureFactory::LoadTextureFromSourceFile(const std::filesystem::path& path, bool verticalFlip)
	{
		HZR_PROFILE_FUNCTION();
		stbi_set_flip_vertically_on_load(verticalFlip);
		return File::GetFileExtension(path) == ".hdr" ? LoadFloatTextureFromFile(path) : LoadByteTextureFromFile(path);
	}

	TextureHeader TextureFactory::LoadTextureFromMemory(Buffer buffer)
	{
		int w, h, channels;
		constexpr int desired = 4;

		stbi_uc* data = stbi_load_from_memory((stbi_uc*)buffer.Data, buffer.Size, &w, &h, &channels, desired);
		HZR_CORE_ASSERT(data, "Data not loaded correctly");

		TextureHeader header = {
			.Extent = {
				.Width = (uint32_t)w,
				.Height = (uint32_t)h,
				.Depth = 1
			},
			.Channels = desired,
			.Format = HazardRenderer::ImageFormat::RGBA,
			.Mips = 1,
			.ImageData = Buffer::Copy(data, w * h * desired),
		};

		stbi_image_free(data);

		return header;
	}

	TextureHeader TextureFactory::LoadByteTextureFromFile(const std::filesystem::path& path)
	{
		Ref<CachedBuffer> buffer = File::ReadBinaryFile(path);
		return LoadTextureFromMemory({ buffer->GetData(), buffer->GetSize() });
	}

	TextureHeader TextureFactory::LoadFloatTextureFromFile(const std::filesystem::path& path)
	{
		int w, h, channels;
		constexpr int desired = 4;

		Ref<CachedBuffer> buffer = File::ReadBinaryFile(path.lexically_normal());

		float* data = stbi_loadf_from_memory((stbi_uc*)buffer->GetData(), buffer->GetSize(), &w, &h, &channels, desired);
		HZR_CORE_ASSERT(data, "Data not loaded correctly");

		TextureHeader header = {
			.Extent = {
				.Width = (uint32_t)w,
				.Height = (uint32_t)h,
				.Depth = 1
			},
			.Channels = desired,
			.Format = HazardRenderer::ImageFormat::RGBA32F,
			.Mips = 1,
			.ImageData = Buffer::Copy(data, w * h * desired * sizeof(float)),
		};

		stbi_image_free(data);

		return header;
	}

}
