#pragma once
#include "Backend/Core/Image2D.h"
#ifdef HZR_INCLUDE_VULKAN
#include "Buffer.h"
#include <map>
#include <vector>
#include "vk_mem_alloc.h"


namespace HazardRenderer::Vulkan 
{
	struct VulkanImageInfo
	{
		VkImage Image = nullptr;
		VkImageView ImageView = nullptr;
		VkSampler Sampler = nullptr;
		VmaAllocation MemoryAlloc = nullptr;
		ImageUsage Usage;
		ImageFormat Format;
	};

	class VulkanImage2D : public Image2D {
	public:
		VulkanImage2D(Image2DCreateInfo* info);
		~VulkanImage2D();
		
		void Invalidate() override;
		void RT_Invalidate();
		void Release() override;

		void Resize(uint32_t width, uint32_t height);

		uint32_t GetWidth() { return m_Width; };
		uint32_t GetHeight() { return m_Height; };
		float GetAspectRatio() { return (float)m_Width / (float)m_Height; }
		ImageFormat GetFormat() const { return m_Info.Format; };

		Buffer GetBuffer() const { return m_Buffer; };
		Buffer& GetBuffer() { return m_Buffer; };
		
		VulkanImageInfo GetImageInfo() { return m_Info; }
		const VkDescriptorImageInfo& GetDescriptor() { return m_DescriptorImageInfo; }

		uint32_t GetID() const { return m_ID; }

		void UpdateDescriptor();
	private:
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_Mips = 1;
		uint32_t m_ID;
		
		VulkanImageInfo m_Info;

		std::vector<VkImageView> m_PerLayerImageViews;
		std::map<uint32_t, VkImageView> m_MipImageViews;
		VkDescriptorImageInfo m_DescriptorImageInfo = {};

		Buffer m_Buffer;
	};
}
#endif