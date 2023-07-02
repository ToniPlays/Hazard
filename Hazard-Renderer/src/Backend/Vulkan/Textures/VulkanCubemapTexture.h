#pragma once

#include "Backend/Core/Texture.h"
#ifdef HZR_INCLUDE_VULKAN

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace HazardRenderer::Vulkan
{
	class VulkanCubemapTexture : public CubemapTexture {
		friend class VulkanRenderCommandBuffer;

	public:
		VulkanCubemapTexture() = default;
		VulkanCubemapTexture(CubemapTextureCreateInfo* createInfo);

		void Bind(uint32_t slot = 0) const;

		TextureType GetType() const { return TextureType::CubemapTexture; };

		ImageFormat GetFormat() const override { return m_Format; };
		uint32_t GetWidth() const override { return m_Width; };
		uint32_t GetHeight() const override { return m_Height; }
		glm::uvec2 GetSize() const override { return { m_Width, m_Height }; };
		uint32_t GetMipLevels() const override { return m_MipLevels; };
		const std::string& GetPath() const override { return m_FilePath; };

		Ref<Image2D> GetSourceImage() override { return m_SourceImage; }

		//Vulkan specific
		VkDescriptorImageInfo GetImageDescriptor() { return m_ImageDescriptor; }
		VkImage GetVulkanImage() { return m_Image; }

		void GenerateMipmaps_RT(VkCommandBuffer commandBuffer, VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		void SetImageData(const Buffer& buffer);
	private:
		void GenerateFromCubemap(CubemapGen& generationData);

		void UploadImageData_RT();

		void CreateImageView_RT();
		void CreateSampler_RT();

		void SetImageLayout(VkImageLayout layout) { m_ImageDescriptor.imageLayout = layout; };
	private:
		ImageFormat m_Format = ImageFormat::None;
		ImageUsage m_Usage = ImageUsage::None;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_MipLevels = 0;

		Buffer m_LocalBuffer;

		std::string m_FilePath = "";
		std::string m_DebugName = "";

		Ref<Image2D> m_SourceImage = nullptr;

		VkDescriptorImageInfo m_ImageDescriptor;
		VkImage m_Image = VK_NULL_HANDLE;
		VmaAllocation m_Allocation = nullptr;
	};
}
#endif
