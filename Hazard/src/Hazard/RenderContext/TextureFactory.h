
#include "Core/Core.h"
#include "Buffer/Buffer.h"
#include "HazardRendererCore.h"
#include <filesystem>
#include <iostream>

namespace Hazard
{
	struct TextureHeader 
	{
		HazardRenderer::Extent Extent;
		uint32_t Dimensions = 0;
		uint32_t Channels = 0;
		HazardRenderer::ImageFormat Format = HazardRenderer::ImageFormat::None;
		uint32_t Mips = 0;
		Buffer ImageData;

		bool IsValid() { return ImageData.Data && ImageData.Size != 0; }
	};

	class TextureFactory 
	{
	public:
		static TextureHeader LoadTextureFromSourceFile(const std::filesystem::path& path, bool verticalFlip = false);
		static uint32_t PixelSize(const HazardRenderer::ImageFormat& format);

	private:
		static TextureHeader LoadByteTextureFromFile(const std::filesystem::path& path);
		static TextureHeader LoadFloatTextureFromFile(const std::filesystem::path& path);
	};
}