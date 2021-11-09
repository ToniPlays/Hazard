#pragma once

#include "Image/VulkanImage2D.h"
#include "Hazard/Rendering/Texture.h"
#include "Hazard/Rendering/TextureFactory.h"

namespace Hazard::Rendering::Vulkan {

	class VulkanTexture2D : public Texture2D 	
	{
	public:
		VulkanTexture2D(Texture2DCreateInfo* info);
		~VulkanTexture2D();

		uint32_t GetWidth() const override { return m_Header.Width; };
		uint32_t GetHeight() const override { return m_Header.Height; };
		void Bind(uint32_t slot = 0) const override;

		void Invalidate();

		void Resize(uint32_t width, uint32_t height) {};
		Buffer GetWriteBuffer() { return Buffer(); };
		bool Loaded() const { return m_Header.ImageData.Data; };
		ImageFormat GetFormat() const override { return m_Format; }

		uint32_t GetID() const override { return m_ID; }

		float GetAspectRatio() { return (float)m_Header.Width / (float)m_Header.Height; }
		Buffer GetBuffer() const { return Buffer(); };
		Buffer& GetBuffer() { return Buffer(); }
		Ref<VulkanImage2D> GetImage() { return m_Image; }

	private:
		bool LoadImageFromFile(const std::string& path);

	private:
		TextureHeader m_Header;
		uint32_t m_ID;
		Ref<VulkanImage2D> m_Image;
		ImageFormat m_Format;
		ImageUsage m_Usage;
	};
}