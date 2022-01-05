#pragma once

#include "Hazard/Assets/Asset.h"
#include "Image/Image.h"

namespace Hazard::Rendering 
{
	struct Texture2DCreateInfo
	{
		//Relative to "res" folder
		std::filesystem::path FilePath;
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Mips = 1;
		ImageWrap WrapMode = ImageWrap::Repeat;
		TextureFilter* Filter = nullptr;
		ImageUsage Usage = ImageUsage::None;
		ImageFormat Format;
		bool ClearLocalBuffer = true;
		void* Data = nullptr;
	};

	class Texture : public Asset 
	{
	public:
		virtual ~Texture() {};

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual ImageFormat GetFormat() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
        
	};
	class Texture2D : public Texture
	{
	public:
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual Buffer GetWriteBuffer() = 0;
		virtual bool Loaded() const = 0;

		static Ref<Texture2D> Create(Texture2DCreateInfo* info);
	};
}
