
#include <hzrpch.h>
#include "TextureFactory.h"
#include "File.h"

#include "vendor/stb_image.h"

namespace Hazard
{
	struct FileHeader
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Channels;
		uint64_t DataSize;
	};

	bool TextureFactory::SaveTextureToCache(const AssetHandle& handle, const TextureHeader& header)
	{
		HZR_PROFILE_FUNCTION();
		std::filesystem::path cacheDir = GetCacheFile(handle);

		if (!File::DirectoryExists(cacheDir.parent_path()))
			File::CreateDir(cacheDir.parent_path());

		size_t dataSize;
		uint8_t* textureData = nullptr;

		if (header.Channels == 4)
		{
			dataSize = sizeof(FileHeader) + header.ImageData.Size;
			textureData = new uint8_t[dataSize];

			FileHeader fileHeader = {};
			fileHeader.Width = header.Width;
			fileHeader.Height = header.Height;
			fileHeader.Channels = header.Channels;
			fileHeader.DataSize = header.ImageData.Size;

			memcpy(textureData, &fileHeader, sizeof(FileHeader));
			memcpy(textureData + sizeof(FileHeader), header.ImageData.Data, header.ImageData.Size);

			bool result = File::WriteBinaryFile(cacheDir, textureData, dataSize);

			delete[] textureData;
			return result;
		}
		return false;
	}

	bool TextureFactory::CacheFileChanged(const std::string& path)
	{
		return false;
	}

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

	TextureHeader TextureFactory::LoadTextureFromCache(const AssetHandle& handle)
	{
		HZR_PROFILE_FUNCTION();
		Timer timer;
		auto cacheFile = GetCacheFile(handle);

		Buffer data = File::ReadBinaryFile(cacheFile);

		TextureHeader header = {};

		if (!data)
		{
			data.Release();
			return header;
		}

		FileHeader fileHeader = Buffer::Get<FileHeader>(data.Data);

		header.Width = fileHeader.Width;
		header.Height = fileHeader.Height;
		header.Channels = fileHeader.Channels;
		header.ImageData = Buffer::Copy(data.Data, fileHeader.DataSize, sizeof(FileHeader));

		data.Release();
		std::cout << "Image load from cache " << cacheFile << " took " << timer.ElapsedMillis() << " ms" << std::endl;
		return header;
	}
	TextureHeader TextureFactory::LoadTextureFromSourceFile(const std::string& path, bool verticalFlip)
	{
		Timer timer;
		HZR_PROFILE_FUNCTION();
		HZR_CORE_ASSERT(File::Exists(path), "Source file does not exist");
		TextureHeader header = {};

		int w, h, channels;
		constexpr int desired = 4;
		{
			stbi_set_flip_vertically_on_load(verticalFlip);

			stbi_uc* data = stbi_load(path.c_str(), &w, &h, &channels, desired);
			HZR_CORE_ASSERT(data, "Data not loaded correctly");
			header.ImageData = Buffer(data, w * h * desired);
		}

		header.Width = w;
		header.Height = h;
		header.Channels = desired;

		std::cout << "Image load from source " << path << " took " << timer.ElapsedMillis() << " ms" << std::endl;

		return header;
	}
	TextureHeader TextureFactory::GetFromCacheOrReload(const AssetHandle& handle, const std::filesystem::path& path)
	{
		HZR_ASSERT(handle != INVALID_ASSET_HANDLE, "Handle cannot be INVALID");

		if (TextureFactory::CacheStatus(handle) == CacheStatus::Exists)
			return TextureFactory::LoadTextureFromCache(handle);

		TextureHeader header = TextureFactory::LoadTextureFromSourceFile(path.string(), true);
		TextureFactory::SaveTextureToCache(handle, header);

		return header;
	}
}
