#pragma once

#include "Backend/Core/Texture/Texture.h"
#ifdef HZR_INCLUDE_VULKAN

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace HazardRenderer::Vulkan 
{
	class VulkanCubemapTexture : public CubemapTexture {
	public:
		VulkanCubemapTexture() = default;
		VulkanCubemapTexture(CubemapTextureCreateInfo* createInfo);

		void Bind(uint32_t slot = 0) const;

		ImageFormat GetFormat() const {	return m_Format; };
		uint32_t GetWidth() const { return m_Width; };
		uint32_t GetHeight() const { return m_Height; }
		glm::uvec2 GetSize() const { return { m_Width, m_Height }; };
		const std::string& GetPath() const { return m_FilePath;	};
		VkDescriptorImageInfo GetImageDescriptor() { return m_ImageDescriptor; }

		Ref<Image2D> GetSourceImage() { return m_SourceImage; }

	private:
		void GenerateFromEquirectangular(Ref<Image2D> sourceImage);
		void GenerateFromCubemap(CubemapGen& generationData);

		void CreateImageView_RT();
		void CreateSampler_RT();
		void GenerateMipmaps_RT(VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	private:
		ImageFormat m_Format = ImageFormat::None;
		ImageUsage m_Usage = ImageUsage::None;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_MipLevels = 0;
		std::string m_FilePath = "";
		std::string m_DebugName = "";
		

		Ref<Image2D> m_SourceImage = nullptr;

		VkDescriptorImageInfo m_ImageDescriptor;
		VkImage m_Image;
		VmaAllocation m_Allocation = nullptr;
	};
}
#endif