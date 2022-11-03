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
		static void SetCacheLocation(const std::filesystem::path& path) { s_CacheDirectory = path; }
		static CacheStatus CacheStatus(const AssetHandle& handle);
		static TextureHeader LoadTextureFromSourceFile(const std::filesystem::path& path, bool verticalFlip = false);
		static std::filesystem::path GetCacheFile(const AssetHandle& handle);
		static uint32_t PixelSize(const HazardRenderer::ImageFormat& format);

	private:
		static inline std::filesystem::path s_CacheDirectory = "Library/Image/";
	};
}