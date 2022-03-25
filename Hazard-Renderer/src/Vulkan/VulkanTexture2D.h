//#pragma once
// #ifdef HZR_INCLUDE_VULKAN
//
//#include "Image/VulkanImage2D.h"
////#include "Hazard/Rendering/Texture.h"
//#include "Hazard/Rendering/Loaders/TextureFactory.h"
//
//namespace Hazard::Rendering::Vulkan {
//
//	class VulkanTexture2D : public Texture2D 	
//	{
//	public:
//		VulkanTexture2D(Texture2DCreateInfo* info);
//		~VulkanTexture2D();
//
//		uint32_t GetWidth() const override { return m_Header.Width; };
//		uint32_t GetHeight() const override { return m_Header.Height; };
//		void Bind(uint32_t slot = 0) const override;
//
//		void Invalidate();
//
//		void Resize(uint32_t width, uint32_t height) {};
//		Buffer GetWriteBuffer() { return Buffer(); };
//		bool Loaded() const { return m_Header.ImageData.Data; };
//		ImageFormat GetFormat() const override { return m_Format; }
//
//		float GetAspectRatio() { return (float)m_Header.Width / (float)m_Header.Height; }
//		Buffer GetBuffer() const { return m_Buffer; };
//		Buffer& GetBuffer() { return m_Buffer; }
//		Ref<VulkanImage2D> GetImage() { return m_Image; }
//
//	private:
//		bool LoadImageFromFile(const std::string& path);
//
//	private:
//		TextureHeader m_Header;
//		Ref<VulkanImage2D> m_Image;
//		ImageFormat m_Format;
//		ImageUsage m_Usage;
//		ImageWrap m_WrapMode;
//		TextureFilter m_Filter;
//		Buffer m_Buffer;
//	};
//}