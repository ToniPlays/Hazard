#pragma once

#include "Backend/Core/Texture/Image2D.h"
#ifdef HZR_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

#include <map>

namespace HazardRenderer::Vulkan
{
	struct VulkanImageInfo 
	{
		VkImage Image = VK_NULL_HANDLE;
		VkImageView ImageView = VK_NULL_HANDLE;
		VkSampler Sampler = VK_NULL_HANDLE;
		VmaAllocation Allocation = nullptr;
	};

	class VulkanImage2D : public Image2D 
	{
	public:
		VulkanImage2D(Image2DCreateInfo* info);
		~VulkanImage2D();

		void Invalidate() override;
		void Release() override;
		void Release_RT();

		void Resize_RT(uint32_t width, uint32_t height);

		uint32_t GetWidth() override { return m_Width; };
		uint32_t GetHeight() override { return m_Height; };
		ImageFormat GetFormat() const override { return m_Format; }

		virtual float GetAspectRatio() override { return (float)m_Width / (float)m_Height; };
		virtual Buffer GetBuffer() const override { return m_LocalBuffer; };
		virtual const Buffer& GetBuffer() override { return m_LocalBuffer; };

		//Vulkan specific
		VulkanImageInfo& GetImageInfo() { return m_Info; }
		const VulkanImageInfo& GetImageInfo() const { return m_Info; }
		VkDescriptorImageInfo GetImageDescriptor() { return m_ImageDescriptor; }
		uint32_t GetLayerCount() { return 1; }

		VkImageView GetLayerImageView(uint32_t layer) { return m_LayerImageViews[layer]; }

		void Invalidate_RT();
		void UpdateDescriptor();

		void UploadImageData_RT(Buffer data);

	private:

	private:
		std::string m_DebugName;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_Mips;
		ImageFormat m_Format;
		ImageUsage m_Usage;
		Buffer m_LocalBuffer;

		//Vulkan Specific
		VulkanImageInfo m_Info;

		std::vector<VkImageView> m_LayerImageViews;
		std::map<uint32_t, VkImageView> m_PerMipImageView;

		VkDescriptorImageInfo m_ImageDescriptor;

	};
}
#endif