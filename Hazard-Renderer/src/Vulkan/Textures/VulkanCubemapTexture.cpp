
#include "VulkanCubemapTexture.h"
#ifdef HZR_INCLUDE_VULKAN
#include "File.h"

#include "Core/Renderer.h"
#include "Core/Rendering/Pipeline.h"
#include "../VulkanContext.h"
#include "../VkUtils.h"
#include "../VulkanRenderCommandBuffer.h"
#include "spdlog/fmt/fmt.h"
#include "VulkanImage2D.h"


#include "vendor/stb_image.h"

namespace HazardRenderer::Vulkan
{
	VulkanCubemapTexture::VulkanCubemapTexture(CubemapTextureCreateInfo* createInfo) : m_Format(createInfo->Format)
	{
		HZR_PROFILE_FUNCTION();

		HZR_ASSERT(createInfo->Usage != ImageUsage::None, "Cannot create cubemap with no usage");
		HZR_ASSERT(!createInfo->DebugName.empty(), "");

		m_DebugName = createInfo->DebugName;
		m_Usage = createInfo->Usage;

		m_Width = createInfo->Width;
		m_Height = createInfo->Height;

		m_ImageDescriptor.imageView = VK_NULL_HANDLE;
		m_ImageDescriptor.sampler = VK_NULL_HANDLE;

		m_MipLevels = createInfo->MaxMips == 1 ? 1 : glm::min(Math::GetBaseLog(m_Width), createInfo->MaxMips);

		Ref<VulkanCubemapTexture> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {

			const auto& device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

			VulkanAllocator allocator("VulkanCubemap");

			VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT;
			usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
			usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			usage |= VK_IMAGE_USAGE_STORAGE_BIT;

			VkImageCreateInfo imageCreateInfo = {};
			imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
			imageCreateInfo.format = VkUtils::VulkanImageFormat(instance->m_Format);
			imageCreateInfo.extent.width = instance->m_Width;
			imageCreateInfo.extent.height = instance->m_Height;
			imageCreateInfo.extent.depth = 1;
			imageCreateInfo.mipLevels = instance->m_MipLevels;
			imageCreateInfo.arrayLayers = 6;
			imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageCreateInfo.usage = usage;
			imageCreateInfo.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

			instance->m_ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

			instance->m_Allocation = allocator.AllocateImage(imageCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, instance->m_Image);
			VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE, fmt::format("VkCubemap {0}", instance->m_DebugName), instance->m_Image);

			VkCommandBuffer layoutCmd = VulkanContext::GetLogicalDevice()->GetCommandBuffer(true);

			VkImageSubresourceRange subRange = {};
			subRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subRange.baseMipLevel = 0;
			subRange.levelCount = instance->m_MipLevels;
			subRange.baseArrayLayer = 0;
			subRange.layerCount = 6;

			VkUtils::SetImageLayout(layoutCmd, instance->m_Image, VK_IMAGE_LAYOUT_UNDEFINED, instance->m_ImageDescriptor.imageLayout, subRange);

			VulkanContext::GetLogicalDevice()->FlushCommandBuffer(layoutCmd);
			instance->CreateImageView_RT();
		});

		if (createInfo->Data)
		{
			UploadImageData(createInfo->Data);
			if (m_MipLevels > 1)
				GenerateMipmaps();
		}
	}
	VulkanCubemapTexture::~VulkanCubemapTexture()
	{
		HZR_PROFILE_FUNCTION();
		Renderer::SubmitResourceFree([image = m_Image, alloc = m_Allocation]() mutable {

			VulkanAllocator allocator("VulkanCubemapTexture");
			allocator.DestroyImage(image, alloc);
		});
	}
	void VulkanCubemapTexture::UploadImageData(Buffer imageData)
	{
		Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::Create("UploadImage", DeviceQueue::TransferBit, 1);
		cmdBuffer->Begin();

		ImageCopyRegion region = {};
		region.Extent = { m_Width, m_Height, 6 };
		region.X = 0;
		region.Y = 0;
		region.Z = 0;
		region.Data = imageData.Data;
		region.DataSize = imageData.Size;

		ImageMemoryInfo barrier = {};
		barrier.Image = (Image*)this;
		barrier.BaseLayer = 0;
		barrier.LayerCount = 6;
		barrier.BaseMip = 0;
		barrier.MipCount = 1;
		barrier.SrcLayout = IMAGE_LAYOUT_UNDEFINED;
		barrier.DstLayout = IMAGE_LAYOUT_TRANSFER_DST;

		cmdBuffer->ImageMemoryBarrier(barrier);
		cmdBuffer->CopyToImage((Image*)this, region);

		barrier.SrcLayout = IMAGE_LAYOUT_TRANSFER_DST;
		barrier.DstLayout = IMAGE_LAYOUT_SHADER_READ_ONLY;
		cmdBuffer->ImageMemoryBarrier(barrier);

		cmdBuffer->End();
		cmdBuffer->Submit();
	}
	void VulkanCubemapTexture::CreateImageView_RT()
	{
		HZR_PROFILE_FUNCTION();
		const auto& device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

		if (m_ImageDescriptor.imageView)
			vkDestroyImageView(device, m_ImageDescriptor.imageView, nullptr);

		VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT;
		usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		usage |= VK_IMAGE_USAGE_STORAGE_BIT;

		VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
		viewInfo.format = VkUtils::VulkanImageFormat(m_Format);
		viewInfo.flags = 0;
		viewInfo.subresourceRange = {};
		viewInfo.subresourceRange.aspectMask = aspectMask;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = m_MipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 6;
		viewInfo.image = m_Image;

		VK_CHECK_RESULT(vkCreateImageView(device, &viewInfo, nullptr, &m_ImageDescriptor.imageView), "Failed to create VkImageView");
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("VkImageView {0}", m_DebugName), m_ImageDescriptor.imageView);
	}
	void VulkanCubemapTexture::RegenerateMips()
	{
		GenerateMipmaps(m_ImageDescriptor.imageLayout);
	}
	void VulkanCubemapTexture::GenerateMipmaps(VkImageLayout imageLayout)
	{
		std::cout << m_DebugName << std::endl;
		Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::Create("Image gen mip", DeviceQueue::TransferBit, 1);

		cmdBuffer->Begin();
		{
			ImageMemoryInfo barrier = {};
			barrier.Image = (Image*)this;
			barrier.BaseLayer = 0;
			barrier.LayerCount = 6;
			barrier.BaseMip = 0;
			barrier.MipCount = 1;
			barrier.SrcLayout = IMAGE_LAYOUT_GENERAL;
			barrier.DstLayout = IMAGE_LAYOUT_TRANSFER_SRC;

			cmdBuffer->ImageMemoryBarrier(barrier);
		}
		{
			ImageMemoryInfo barrier = {};
			barrier.Image = (Image*)this;
			barrier.BaseLayer = 0;
			barrier.LayerCount = 6;
			barrier.BaseMip = 1;
			barrier.MipCount = m_MipLevels - 1;
			barrier.SrcLayout = IMAGE_LAYOUT_UNDEFINED;
			barrier.DstLayout = IMAGE_LAYOUT_TRANSFER_DST;

			cmdBuffer->ImageMemoryBarrier(barrier);
		}

		for (uint32_t face = 0; face < 6; face++)
		{
			for (uint32_t mip = 1; mip < m_MipLevels; mip++)
			{
				BlitImageInfo blit = {};
				blit.Image = (Image*)this;
				blit.SrcExtent = { m_Width >> (mip - 1), m_Height >> (mip - 1), 1 };
				blit.SrcLayer = face;
				blit.SrcMip = mip - 1;
				blit.SrcLayout = IMAGE_LAYOUT_TRANSFER_SRC;

				blit.DstExtent = { m_Width >> mip, m_Height >> mip, 1 };
				blit.DstLayer = face;
				blit.DstMip = mip;
				blit.DstLayout = IMAGE_LAYOUT_TRANSFER_DST;

				cmdBuffer->BlitImage(blit);

				ImageMemoryInfo barrier = {};
				barrier.Image = (Image*)this;
				barrier.BaseLayer = face;
				barrier.LayerCount = 1;
				barrier.BaseMip = mip;
				barrier.MipCount = 1;
				barrier.SrcLayout = IMAGE_LAYOUT_TRANSFER_DST;
				barrier.DstLayout = IMAGE_LAYOUT_TRANSFER_SRC;

				cmdBuffer->ImageMemoryBarrier(barrier);
			}
		}

		ImageMemoryInfo barrier = {};
		barrier.Image = (Image*)this;
		barrier.BaseLayer = 0;
		barrier.LayerCount = 6;
		barrier.BaseMip = 0;
		barrier.MipCount = m_MipLevels;
		barrier.SrcLayout = IMAGE_LAYOUT_TRANSFER_SRC;
		barrier.DstLayout = IMAGE_LAYOUT_GENERAL;

		cmdBuffer->ImageMemoryBarrier(barrier);

		cmdBuffer->End();
		cmdBuffer->Submit();
	}
}
#endif
