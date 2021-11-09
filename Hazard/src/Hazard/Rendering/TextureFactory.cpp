#pragma once

#include <hzrpch.h>
#include "TextureFactory.h"

#include <stb_image.h>

namespace Hazard::Rendering
{

	bool TextureFactory::TextureCacheExists(const std::string& path)
	{
		auto cacheDir = GetFileCachePath(path);
		return File::Exists(cacheDir);
	}

	std::filesystem::path TextureFactory::GetFileCachePath(const std::string& path)
	{
		std::string dir = m_CacheDirectory + "/" + path + ".hzrche";
		return std::filesystem::path(dir);
	}

	std::filesystem::path TextureFactory::GetFileSourcePath(const std::string& path)
	{
		return std::filesystem::path("res/" + path);
	}

	bool TextureFactory::SaveTextureToCache(const std::string& path, const TextureHeader& header)
	{
		std::filesystem::path cacheDir = GetFileCachePath(path);

		HZR_CORE_INFO("Creating texture cache {0}", path);

		if (!File::DirectoryExists(cacheDir.parent_path()))
			File::CreateDir(cacheDir.parent_path());
		
		uint32_t dataSize;
		uint32_t* textureData;
		if (header.Channels == 4) {

			dataSize = sizeof(TextureHeader) + header.ImageData.Size;
			textureData = new uint32_t[dataSize];

			memcpy(textureData, &header, sizeof(TextureHeader));
			memcpy(textureData + sizeof(TextureHeader), header.ImageData.Data, header.ImageData.Size);
		}
		else 
		{
			return false;
		}

		return File::WriteBinaryFile(cacheDir, textureData, dataSize);
	}

	bool TextureFactory::CacheFileChanged(const std::string& path)
	{
		auto sourceFile = GetFileSourcePath(path);
		auto cacheFile = GetFileCachePath(path);

		if (!File::Exists(cacheFile)) return false;

		return File::IsNewerThan(sourceFile, cacheFile);
	}

	TextureHeader TextureFactory::LoadTextureFromCache(const std::string& path)
	{
		Timer timer;
		auto cacheFile = GetFileCachePath(path);

		size_t fileSize = 0;
		uint32_t* data = File::ReadBinaryFile(cacheFile, fileSize);

		TextureHeader header = {};
		memcpy(&header, data, sizeof(TextureHeader));

		header.ImageData = Buffer::Copy(data, header.DataSize, sizeof(TextureHeader));

		HZR_CORE_INFO("Texture {0} loaded from cache in {1} ms", path, timer.ElapsedMillis());
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
		return header;
	}
	TextureHeader TextureFactory::LoadTextureFromSourceFile(const std::string& path, bool verticalFlip)
	{
		std::string sourceFile = "res/" + path;
		TextureHeader header = {};

		int w, h, channels;
		{
			Timer timer;
			stbi_set_flip_vertically_on_load(verticalFlip);
			void* data = stbi_load(sourceFile.c_str(), &w, &h, &channels, 4);
			header.ImageData = Buffer(data, w * h * channels);
			HZR_CORE_INFO("Texture \"{0}\" loaded from source in {1} ms", sourceFile, timer.ElapsedMillis());
		}

		header.Width = w;
		header.Height = h;
		header.Channels = channels;
		header.DataSize = w * h * channels;

		return header;
	}
}
