#pragma once

#include "HazardRendererCore.h"

namespace HazardRenderer 
{
	class CubemapTexture;

	struct CubemapTextureCreateInfo
	{
		std::string FilePath;
		uint32_t Width;
		uint32_t Height;
		Buffer Data;
		uint32_t Mips = 1;
		ImageUsage Usage = ImageUsage::None;
		ImageFormat Format = ImageFormat::None;
		Ref<CubemapTexture> pCubemap = nullptr;
	};


	class Texture : public HZR_RESOURCE_HANDLER {
	public:

		virtual ~Texture() = default;
		virtual void Bind(uint32_t slot = 0) const = 0;
		
		virtual ImageFormat GetFormat() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual glm::uvec2 GetSize() const = 0;

		virtual TextureType GetType() const = 0;
	};

	class CubemapTexture : public Texture {
	public:

		static Ref<CubemapTexture> Create(CubemapTextureCreateInfo* createInfo);
		virtual const std::string& GetPath() const = 0;
		virtual TextureType GetType() const override { return TextureType::CubemapTexture; }

	};
}