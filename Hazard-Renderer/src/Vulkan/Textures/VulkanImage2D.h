#pragma once

#include "Core/Rendering/Image2D.h"

#ifdef HZR_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

#include <map>

namespace HazardRenderer::Vulkan
{
	class VulkanImage2D : public Image2D 
	{
	public:
		VulkanImage2D(Image2DCreateInfo* info);
		~VulkanImage2D();

		void Invalidate() override;
		void Release() override;
		void Release_RT();

		void Resize_RT(uint32_t width, uint32_t height);

		Buffer ReadPixels(const ImageCopyRegion& region) { return Buffer(); };

		TextureType GetType() const { return TextureType::Image2D; };
		uint32_t GetWidth() override { return m_Info.Extent.Width; };
		uint32_t GetHeight() override { return m_Info.Extent.Height; };
		ImageFormat GetFormat() const override { return m_Info.Format; }
		const std::string& GetDebugName() const override { return m_Info.DebugName; };
		virtual uint32_t GetMipLevels() const { return m_Info.Mips; };

		virtual float GetAspectRatio() override { return (float)m_Info.Extent.Width / (float)m_Info.Extent.Height; };
		virtual Buffer GetBuffer() const override { return m_LocalBuffer; };
		virtual const Buffer& GetBuffer() override { return m_LocalBuffer; };

		//Vulkan specific
		VkDescriptorImageInfo GetImageDescriptor() { return m_ImageDescriptor; }
		VkImage GetVulkanImage() { return m_Image; }
		uint32_t GetLayerCount() { return 1; }
		bool IsValid() 
		{ 
			return m_ImageDescriptor.imageView != VK_NULL_HANDLE
				&& m_ImageDescriptor.imageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
				&& m_IsValid;
		}

		VkImageView GetLayerImageView(uint32_t layer) { return m_LayerImageViews[layer]; }
		void SetImageLayout(VkImageLayout layout) { m_ImageDescriptor.imageLayout = layout; };

		void Invalidate_RT();
		void UploadImageData_RT(Buffer data, VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	private:

		void CreateImageView_RT();
		//void CreateSampler_RT();
		void GenerateMips_RT();

	private:
		Image2DCreateInfo m_Info;
		Buffer m_LocalBuffer;

		//Vulkan Specific

		std::vector<VkImageView> m_LayerImageViews;
		std::map<uint32_t, VkImageView> m_PerMipImageView;

		VkDescriptorImageInfo m_ImageDescriptor;
		VkImage m_Image = VK_NULL_HANDLE;
		VmaAllocation m_Allocation = nullptr;

		bool m_IsValid = false;
	};
}
#endif