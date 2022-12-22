#pragma once

#include "Backend/Core/Pipeline/Pipeline.h"
#include "Image.h"
#include "Buffer/Buffer.h"

#include "glm/glm.hpp"

namespace HazardRenderer 
{
	class CubemapTexture;

	struct CubemapGen
	{
		std::string OutputImageName;
		Ref<CubemapTexture> pCubemap = nullptr;
		Ref<Pipeline> Pipeline = nullptr;
	};

	struct CubemapTextureCreateInfo
	{
		std::string DebugName;
		std::string FilePath;
		uint32_t Width;
		uint32_t Height;
		Buffer Data;
		uint32_t Mips = 1;
		ImageUsage Usage = ImageUsage::None;
		ImageFormat Format = ImageFormat::None;
		bool FlipOnLoad = false;
		bool GenerateMips = true;
		CubemapGen* pCubemapSrc = nullptr;
	};

	class Texture : public RefCount 
	{
	public:

		virtual ~Texture() = default;
		virtual void Bind(uint32_t slot = 0) const = 0;
		
		virtual ImageFormat GetFormat() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual glm::uvec2 GetSize() const = 0;
		virtual uint32_t GetMipLevels() const = 0;

		virtual TextureType GetType() const = 0;
	};

	class CubemapTexture : public Texture {
	public:

		virtual const std::string& GetPath() const = 0;
		virtual Ref<Image2D> GetSourceImage() = 0;
		virtual TextureType GetType() const override { return TextureType::CubemapTexture; }

		static Ref<CubemapTexture> Create(CubemapTextureCreateInfo* createInfo);

	};
}
