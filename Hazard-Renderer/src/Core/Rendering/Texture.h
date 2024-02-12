#pragma once

#include "Core/Rendering/Pipeline.h"
#include "Image.h"
#include "Buffer/Buffer.h"

#include "glm/glm.hpp"

namespace HazardRenderer 
{
	class CubemapTexture;

	struct CubemapTextureCreateInfo
	{
		std::string DebugName;
		uint32_t Width;
		uint32_t Height;
		uint32_t MaxMips = 1;
		ImageUsage Usage = ImageUsage::None;
		ImageFormat Format = ImageFormat::None;
		Buffer Data;
	};

	class Texture : public RefCount
	{
	public:

		virtual ~Texture() = default;
		
		virtual ImageFormat GetFormat() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual glm::uvec2 GetSize() const = 0;
		virtual uint32_t GetMipLevels() const = 0;

		virtual TextureType GetType() const = 0;

		virtual void RegenerateMips() = 0;
	};

	class CubemapTexture : public Texture {
	public:

		virtual TextureType GetType() const override { return TextureType::CubemapTexture; }

		static Ref<CubemapTexture> Create(CubemapTextureCreateInfo* createInfo);

	};
}
