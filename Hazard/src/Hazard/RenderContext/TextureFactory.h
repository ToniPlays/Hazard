#pragma once

#include "Core/Core.h"
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
		Buffer ImageData;
		HazardRenderer::ImageFormat Format = HazardRenderer::ImageFormat::None;

		bool IsValid() { return ImageData.Data && ImageData.Size != 0; }
	};

	struct TextureFileHeader
	{
		uint32_t Width, Height;
		uint32_t Dimensions = 2;
		uint32_t Channels = 0;
		HazardRenderer::ImageFormat Format = HazardRenderer::ImageFormat::None;
	};

	class TextureFactory 
	{
	public:
		static TextureHeader LoadTextureFromSourceFile(const std::filesystem::path& path, bool verticalFlip = false);
		static uint32_t PixelSize(const HazardRenderer::ImageFormat& format);
	};
}