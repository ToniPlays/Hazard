
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
		uint32_t DataSize;
	};

	bool TextureFactory::TextureCacheExists(const std::string& path)
	{
		auto cacheDir = GetFileCachePath(path);
		return File::Exists(cacheDir);
	}

	std::filesystem::path TextureFactory::GetFileCachePath(const std::string& path)
	{
		std::string fileName = File::GetName(path);
		std::string dir = m_CacheDirectory + "/" + fileName + ".hzrche";
		return std::filesystem::path(dir);
	}

	std::filesystem::path TextureFactory::GetFileSourcePath(const std::string& path)
	{
		if (std::filesystem::path(path).is_absolute()) 
			return path;
		return std::filesystem::path("res/" + path);
	}

	bool TextureFactory::SaveTextureToCache(const std::string& path, const TextureHeader& header)
	{
		std::filesystem::path cacheDir = GetFileCachePath(path);

		if (!File::DirectoryExists(cacheDir.parent_path()))
			File::CreateDir(cacheDir.parent_path());

		size_t dataSize;
		byte* textureData = nullptr;

		if (header.Channels == 4) {

			dataSize = sizeof(FileHeader) + header.ImageData.Size;
			textureData = new byte[dataSize];

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
		auto sourceFile = GetFileSourcePath(path);
		auto cacheFile = GetFileCachePath(path);

		if (!File::Exists(cacheFile)) return false;

		return File::IsNewerThan(sourceFile, cacheFile);
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

	TextureHeader TextureFactory::LoadTextureFromCache(const std::string& path)
	{
		auto cacheFile = GetFileCachePath(path);

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
		return header;
	}
	TextureHeader TextureFactory::LoadFromCacheIfExists(const std::string& path, bool reloadIfOutdated)
	{
		bool cacheExists = TextureCacheExists(path);
		//bool isNewer = CacheFileChanged(path) && reloadIfOutdated;

		if (!cacheExists)
		{
			TextureHeader header = LoadTextureFromSourceFile(path, true);
			SaveTextureToCache(path, header);
			return header;
		}

		TextureHeader header = LoadTextureFromCache(path);

		if (!header.IsValid()) {
			header = LoadTextureFromSourceFile(path, true);
			SaveTextureToCache(path, header);
		}
			
		return header;
	}
	TextureHeader TextureFactory::LoadTextureFromSourceFile(const std::string& path, bool verticalFlip)
	{
		auto sourceFile = GetFileSourcePath(path);

		TextureHeader header = {};

		int w, h, channels;
		constexpr int desired = 4;
		{
			stbi_set_flip_vertically_on_load(verticalFlip);

			stbi_uc* data = stbi_load(sourceFile.string().c_str(), &w, &h, &channels, desired);
			header.ImageData = Buffer(data, w * h * desired);
		}

		header.Width = w;
		header.Height = h;
		header.Channels = desired;

		return header;
	}
}
