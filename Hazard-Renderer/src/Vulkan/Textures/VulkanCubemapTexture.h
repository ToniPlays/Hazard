#pragma once

#include "Core/Rendering/Texture.h"
#ifdef HZR_INCLUDE_VULKAN

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace HazardRenderer::Vulkan
{
	class VulkanCubemapTexture : public CubemapTexture {
		friend class VulkanRenderCommandBuffer;

	public:
		VulkanCubemapTexture() = delete;
		VulkanCubemapTexture(CubemapTextureCreateInfo* createInfo);
		~VulkanCubemapTexture();

		TextureType GetType() const { return TextureType::CubemapTexture; };

		ImageFormat GetFormat() const override { return m_Format; };
		uint32_t GetWidth() const override { return m_Width; };
		uint32_t GetHeight() const override { return m_Height; }
		glm::uvec2 GetSize() const override { return { m_Width, m_Height }; };
		uint32_t GetMipLevels() const override { return m_MipLevels; };
		void RegenerateMips() override;

		//Vulkan specific
		VkDescriptorImageInfo GetImageDescriptor() { return m_ImageDescriptor; }
		VkImage GetVulkanImage() { return m_Image; }

		void GenerateMipmaps(VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);


	private:
		void UploadImageData(Buffer imageData);
		void CreateImageView_RT();
		void SetImageLayout(VkImageLayout layout) { m_ImageDescriptor.imageLayout = layout; };

	private:
		ImageFormat m_Format = ImageFormat::None;
		ImageUsage m_Usage = ImageUsage::None;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_MipLevels = 0;

		std::string m_DebugName = "";

		Buffer m_LocalBuffer;

		VkDescriptorImageInfo m_ImageDescriptor;
		VkImage m_Image = VK_NULL_HANDLE;
		VmaAllocation m_Allocation = nullptr;
	};
}
#endif
