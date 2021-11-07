#pragma once

#include <hzrpch.h>
#include "VulkanTexture2D.h"
#include "VulkanContext.h"
#include "VKUtils.h"

#include <stb_image.h>

namespace Hazard::Rendering::Vulkan 
{
	VulkanTexture2D::VulkanTexture2D(Texture2DCreateInfo* info) 
	{
		m_Format = info->Format;
		m_Usage = info->Usage;

		if (info->Data == nullptr) {
			bool loaded = LoadImageFromFile(info->FilePath.string());

			Image2DCreateInfo imageSpecs = {};
			imageSpecs.Format = m_Format;
			imageSpecs.Width = m_Width;
			imageSpecs.Height = m_Height;
			imageSpecs.Mips = 1;

			m_Image = Image2D::Create(&imageSpecs);
			Invalidate();
		}
	}
	VulkanTexture2D::~VulkanTexture2D() 
	{
		if (m_Image) 
			m_Image->Release();
	}
	void VulkanTexture2D::Bind(uint32_t slot) const
	{
		
	}
	void VulkanTexture2D::Invalidate()
	{
		auto device = VulkanContext::GetDevice();
		
		m_Image->Release();
		uint32_t mipCount = 1;

		Image2DCreateInfo imageSpecs = {};
		imageSpecs.Width = m_Image->GetWidth();
		imageSpecs.Height = m_Image->GetHeight();
		imageSpecs.Format = m_Image->GetFormat();

		if (!m_ImageData) 
			imageSpecs.Usage = ImageUsage::Texture;

		Ref<VulkanImage2D> image = m_Image.As<VulkanImage2D>();
		image->RT_Invalidate();

		auto& info = image->GetImageInfo();

		if (m_ImageData) 
		{
			VkDeviceSize size = m_ImageData.Size;

			VkMemoryAllocateInfo memAllocInfo{};
			memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

			VulkanAllocator allocator("Texture2D");

			// Create staging buffer
			VkBufferCreateInfo bufferCreateInfo{};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.size = size;
			bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			VkBuffer stagingBuffer;
			VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

			// Copy data to staging buffer
			uint8_t* destData = allocator.MapMemory<uint8_t>(stagingBufferAllocation);
			memcpy(destData, m_ImageData.Data, size);
			allocator.UnmapMemory(stagingBufferAllocation);

			VkCommandBuffer copyCmd = device->GetCommandBuffer(true);

			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = 1;
			subresourceRange.layerCount = 1;
			
			VkImageMemoryBarrier imageMemoryBarrier{};
			imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.image = info.Image;
			imageMemoryBarrier.subresourceRange = subresourceRange;
			imageMemoryBarrier.srcAccessMask = 0;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

			vkCmdPipelineBarrier(
				copyCmd,
				VK_PIPELINE_STAGE_HOST_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &imageMemoryBarrier);

			VkBufferImageCopy bufferCopyRegion = {};
			bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bufferCopyRegion.imageSubresource.mipLevel = 0;
			bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
			bufferCopyRegion.imageSubresource.layerCount = 1;
			bufferCopyRegion.imageExtent.width = m_Width;
			bufferCopyRegion.imageExtent.height = m_Height;
			bufferCopyRegion.imageExtent.depth = 1;
			bufferCopyRegion.bufferOffset = 0;

			// Copy mip levels from staging buffer
			vkCmdCopyBufferToImage(
				copyCmd,
				stagingBuffer,
				info.Image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&bufferCopyRegion);

			VKUtils::InsertImageMemoryBarrier(copyCmd, info.Image,
				VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, image->GetDescriptor().imageLayout,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				subresourceRange);

			device->FlushCommandBuffer(copyCmd);

			allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
		}
		else 
		{
			VkCommandBuffer transitionCmdBuffer = device->GetCommandBuffer(true);
			VkImageSubresourceRange subResourceRange = {};
			subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subResourceRange.layerCount = 1;
			subResourceRange.levelCount = 1;

			VKUtils::SetImageLayout(transitionCmdBuffer, info.Image, VK_IMAGE_LAYOUT_UNDEFINED, image->GetDescriptor().imageLayout, subResourceRange);
			device->FlushCommandBuffer(transitionCmdBuffer);
		}

		VkSamplerCreateInfo sampler{};
		sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler.maxAnisotropy = 1.0f;
		sampler.magFilter = VKUtils::GetSamplerFilter(FilterMode::Linear);
		sampler.minFilter = VKUtils::GetSamplerFilter(FilterMode::Linear);
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler.addressModeU = VKUtils::GetSamplerWrap(ImageWrap::Repeat);
		sampler.addressModeV = VKUtils::GetSamplerWrap(ImageWrap::Repeat);
		sampler.addressModeW = VKUtils::GetSamplerWrap(ImageWrap::Repeat);
		sampler.mipLodBias = 0.0f;
		sampler.compareOp = VK_COMPARE_OP_NEVER;
		sampler.minLod = 0.0f;
		sampler.maxLod = (float)mipCount;

		sampler.maxAnisotropy = 1.0;
		sampler.anisotropyEnable = VK_FALSE;
		sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		VK_CHECK_RESULT(vkCreateSampler(device->GetDevice(), &sampler, nullptr, &info.Sampler));

		if (!false) {
			VkImageViewCreateInfo view{};
			view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			view.viewType = VK_IMAGE_VIEW_TYPE_2D;
			view.format = VKUtils::GetFormat(m_Format);
			view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
			// The subresource range describes the set of mip levels (and array layers) that can be accessed through this image view
			// It's possible to create multiple image views for a single image referring to different (and/or overlapping) ranges of the image
			view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			view.subresourceRange.baseMipLevel = 0;
			view.subresourceRange.baseArrayLayer = 0;
			view.subresourceRange.layerCount = 1;
			view.subresourceRange.levelCount = mipCount;
			view.image = info.Image;
			VK_CHECK_RESULT(vkCreateImageView(device->GetDevice(), &view, nullptr, &info.ImageView));


			image->UpdateDescriptor();
		}
	}
	bool VulkanTexture2D::LoadImageFromFile(const std::string& path)
	{
		int width, height, channels;

		Timer timer;
		m_ImageData.Data = stbi_load(path.c_str(), &width, &height, &channels, 4);
		HZR_CORE_INFO("VulkanTexture2D Load took {0} ms", timer.ElapsedMillis());
		m_ImageData.Size = width * height * 4;
		m_Format = ImageFormat::RGBA;

		if (!m_ImageData.Data) return false;

		m_Width = width;
		m_Height = height;
		HZR_CORE_INFO("Loaded VulkanTexture2D from {0}", path);
		return true;
	}
}