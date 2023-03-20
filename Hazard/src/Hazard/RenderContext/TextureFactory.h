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

	struct TextureFileHeader
	{
		uint32_t Width, Height;
		uint32_t Dimensions = 2;
		uint32_t Channels;
		HazardRenderer::ImageFormat Format = HazardRenderer::ImageFormat::None;
	};

	class TextureFactory 
	{
	public:
		static TextureHeader LoadTextureFromSourceFile(const std::filesystem::path& path, bool verticalFlip = false);
		static std::filesystem::path GetCacheFile(const AssetHandle& handle);
		static uint32_t PixelSize(const HazardRenderer::ImageFormat& format);
	};
}