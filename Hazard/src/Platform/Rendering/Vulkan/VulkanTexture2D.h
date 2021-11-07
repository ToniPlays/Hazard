#pragma once

#include "Hazard/Rendering/Texture.h"
#include "Image/VulkanImage2D.h"

namespace Hazard::Rendering::Vulkan {

	class VulkanTexture2D : public Texture2D 	
	{
	public:
		VulkanTexture2D(Texture2DCreateInfo* info);
		~VulkanTexture2D();

		uint32_t GetWidth() const override { return m_Width; };
		uint32_t GetHeight() const override { return m_Height; };
		void Bind(uint32_t slot = 0) const override;

		void Invalidate();

		void Resize(uint32_t width, uint32_t height) {};
		Buffer GetWriteBuffer() { return Buffer(); };
		bool Loaded() const { return m_Loaded; };
		ImageFormat GetFormat() const override { return m_Format; }

		uint32_t GetID() const override { return m_ID; }

		float GetAspectRatio() { return (float)m_Width / (float)m_Height; }
		Buffer GetBuffer() const { return Buffer(); };
		Buffer& GetBuffer() { return Buffer(); }

	private:
		bool LoadImageFromFile(const std::string& path);

	private:

		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_ID;
		bool m_Loaded = false;

		Ref<Image2D> m_Image;

		ImageFormat m_Format;
		ImageUsage m_Usage;
		Buffer m_ImageData;

	};
}