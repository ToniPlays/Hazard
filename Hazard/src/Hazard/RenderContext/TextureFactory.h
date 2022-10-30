#pragma once

#include "Backend/Core/Core.h"
#include "Buffer/Buffer.h"
#include "HazardRendererCore.h"
#include <filesystem>
#include <iostream>

namespace Hazard
{
	struct TextureHeader 
	{
		uint32_t Width, Height;
		uint32_t Dimensions = 2;
		uint32_t Channels;
		HazardRenderer::ImageFormat Format = HazardRenderer::ImageFormat::None;
		Buffer ImageData;

		bool IsValid() { return ImageData.Data && ImageData.Size != 0; }
	};

	class TextureFactory 
	{
	public:
		static CacheStatus CacheStatus(const AssetHandle& handle);
		static TextureHeader LoadTextureFromCache(const AssetHandle& handle);
		static TextureHeader LoadTextureFromSourceFile(const std::string& path, bool verticalFlip = false);

		static TextureHeader GetFromCacheOrReload(const AssetHandle& handle, const std::filesystem::path& path);

		static bool SaveTextureToCache(const AssetHandle& handle, const TextureHeader& header);
		static bool CacheFileChanged(const std::string& path);

		static uint32_t PixelSize(const HazardRenderer::ImageFormat& format);
	private:

		static std::filesystem::path GetCacheFile(const AssetHandle& handle);

		static inline std::filesystem::path s_CacheDirectory = "Library/Image/";
	};
}