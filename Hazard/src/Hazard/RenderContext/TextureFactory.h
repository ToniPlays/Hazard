#pragma once

#include "Backend/Core/Core.h"
#include "Buffer.h"
#include "HazardRendererCore.h"
#include <filesystem>
#include <iostream>

namespace Hazard
{
	struct TextureHeader 
	{
		uint32_t Width, Height;
		uint32_t Channels;
		Buffer ImageData;

		bool IsValid() { return ImageData.Data && ImageData.Size != 0; }
	};
	
	class TextureFactory 
	{
	public:
		static bool TextureCacheExists(const std::string& path);
		static TextureHeader LoadTextureFromCache(const std::string& path);
		static TextureHeader LoadTexture(const std::string& path, bool reloadIfOutdated = true);
		static TextureHeader LoadTextureFromSourceFile(const std::string& path, bool verticalFlip = false);
		static std::filesystem::path GetFileCachePath(const std::string& path);
		static std::filesystem::path GetFileSourcePath(const std::string& path);

		static bool SaveTextureToCache(const std::string& path, const TextureHeader& header);
		static bool CacheFileChanged(const std::string& path);
		static uint32_t PixelSize(const HazardRenderer::ImageFormat& format);

	private:
		static inline std::string m_CacheDirectory = "library/Textures/";
	};
}