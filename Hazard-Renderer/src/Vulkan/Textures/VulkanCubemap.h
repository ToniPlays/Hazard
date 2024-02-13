#pragma once

#include "Core/Rendering/Cubemap.h"
#ifdef HZR_INCLUDE_VULKAN

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace HazardRenderer::Vulkan
{
	class VulkanCubemap : public Cubemap {
		friend class VulkanRenderCommandBuffer;

	public:
		VulkanCubemap() = delete;
		VulkanCubemap(CubemapCreateInfo* createInfo);
		~VulkanCubemap();


		const std::string& GetDebugName() const override { return m_DebugName; }
		ImageType GetType() const override { return ImageType::Cubemap; };

		ImageFormat GetFormat() const override { return m_Format; };
		const Extent& GetExtent() const override { return m_Extent; };
		uint32_t GetMipLevels() const override { return m_MipLevels; };
		float GetAspectRatio() const { return (float)m_Extent.Width / (float)m_Extent.Height; }

		void Invalidate() override;
		void Release() override;

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
		Extent m_Extent;
		uint32_t m_MipLevels = 0;

		std::string m_DebugName = "";

		Buffer m_LocalBuffer;

		VkDescriptorImageInfo m_ImageDescriptor;
		VkImage m_Image = VK_NULL_HANDLE;
		VmaAllocation m_Allocation = nullptr;
	};
}
#endif
