#pragma once

#include "Core/Core.h"
#include "Buffer.h"
#include "Image.h"
#include <filesystem>
#include <iostream>

namespace HazardRenderer
{
	struct TextureHeader 
	{
		uint32_t Width, Height;
		uint32_t Channels;
		uint32_t DataSize;
		HazardUtility::Buffer ImageData;

		bool IsValid() { return ImageData.Data && ImageData.Size != 0; }
	};
	
	class TextureFactory 
	{
	public:
		static bool TextureCacheExists(const std::string& path);
		static TextureHeader LoadTextureFromCache(const std::string& path);
		static TextureHeader LoadFromCacheIfExists(const std::string& path, bool reloadIfOutdated = true);
		static TextureHeader LoadTextureFromSourceFile(const std::string& path, bool verticalFlip);
		static std::filesystem::path GetFileCachePath(const std::string& path);
		static std::filesystem::path GetFileSourcePath(const std::string& path);

		static bool SaveTextureToCache(const std::string& path, const TextureHeader& header);
		static bool CacheFileChanged(const std::string& path);
		static uint32_t PixelSize(const ImageFormat& format);

	private:
		static inline std::string m_CacheDirectory = "library/Textures/";
	};
}