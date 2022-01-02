
#include <hzrpch.h>
#include "TextureFactory.h"

#include <stb_image.h>

namespace Hazard::Rendering
{
	struct FileHeader {
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

		uint32_t dataSize;
		byte* textureData = nullptr;

		if (header.Channels == 4) {

			dataSize = sizeof(FileHeader) + header.ImageData.Size;
			textureData = new byte[dataSize];

			FileHeader fileHeader = {};
			fileHeader.Width = header.Width;
			fileHeader.Height = header.Height;
			fileHeader.Channels = header.Channels;
			fileHeader.DataSize = header.DataSize;

			memcpy(textureData, &fileHeader, sizeof(FileHeader));
			memcpy(textureData + sizeof(FileHeader), header.ImageData.Data, header.ImageData.Size);
			bool result = File::WriteBinaryFile(cacheDir, textureData, dataSize);

			delete[] textureData;
			return result;
		}
		HZR_CORE_ASSERT(false, "Texture cache file most likely broken");
		return false;
	}

	bool TextureFactory::CacheFileChanged(const std::string& path)
	{
		auto sourceFile = GetFileSourcePath(path);
		auto cacheFile = GetFileCachePath(path);

		if (!File::Exists(cacheFile)) return false;

		return File::IsNewerThan(sourceFile, cacheFile);
	}

	uint32_t TextureFactory::PixelSize(const ImageFormat& format)
	{
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
		}
	}

	TextureHeader TextureFactory::LoadTextureFromCache(const std::string& path)
	{
		Timer timer;
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
		header.DataSize = fileHeader.DataSize;
		header.ImageData = Buffer::Copy(data.Data, header.DataSize, sizeof(FileHeader));

		HZR_CORE_INFO("Texture {0} loaded from cache in {1} ms", path, timer.ElapsedMillis());

		data.Release();
		return header;
	}
	TextureHeader TextureFactory::LoadFromCacheIfExists(const std::string& path, bool reloadIfOutdated)
	{
		bool cacheExists = TextureCacheExists(path);
		bool isNewer = CacheFileChanged(path) && reloadIfOutdated;

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
		HZR_CORE_ASSERT(File::Exists(sourceFile), "Texture file does not exist");

		TextureHeader header = {};

		int w, h, channels;
		constexpr int desired = 4;
		{
			Timer timer;
			stbi_set_flip_vertically_on_load(verticalFlip);

			stbi_uc* data = stbi_load(sourceFile.string().c_str(), &w, &h, &channels, desired);
			header.ImageData = Buffer(data, w * h * desired);
			HZR_CORE_INFO("Texture \"{0}\" loaded from source in {1} ms", sourceFile.string(), timer.ElapsedMillis());
		}

		header.Width = w;
		header.Height = h;
		header.Channels = desired;
		header.DataSize = w * h * desired;

		return header;
	}
}
