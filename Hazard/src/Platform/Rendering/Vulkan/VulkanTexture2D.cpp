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
		m_WrapMode = info->WrapMode;

		if(info->Filter != nullptr)
			m_Filter = *info->Filter;

		if (info->Data == nullptr) {
			bool loaded = LoadImageFromFile(info->FilePath.string());
			HZR_CORE_ASSERT(loaded, "Failed to load Vulkan Image");
			Image2DCreateInfo imageSpecs = {};
			imageSpecs.Format = m_Format;
			imageSpecs.Width = m_Header.Width;
			imageSpecs.Height = m_Header.Height;
			imageSpecs.Mips = 1;
			imageSpecs.Usage = ImageUsage::Texture;

			m_Image = Image2D::Create(&imageSpecs).As<VulkanImage2D>();
			Invalidate();
		}
		else 
		{
			m_Header.Width = info->Width;
			m_Header.Height = info->Height;
			m_Header.Channels = 4;
			m_Header.DataSize = info->Width * info->Height * 4;
			m_Header.ImageData = Buffer::Copy(info->Data, m_Header.DataSize);

			Image2DCreateInfo imageSpecs = {};
			imageSpecs.Format = m_Format;
			imageSpecs.Width = m_Header.Width;
			imageSpecs.Height = m_Header.Height;
			imageSpecs.Mips = 1;
			imageSpecs.Usage = ImageUsage::Texture;

			m_Image = Image2D::Create(&imageSpecs).As<VulkanImage2D>();
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
		m_Image->RT_Invalidate();

		auto& info = m_Image->GetImageInfo();

		if (m_Header.IsValid())
		{
			VkDeviceSize size = m_Header.DataSize;

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
			memcpy(destData, m_Header.ImageData.Data, size);
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
			bufferCopyRegion.imageExtent.width = m_Header.Width;
			bufferCopyRegion.imageExtent.height = m_Header.Height;
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
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_Image->GetDescriptor().imageLayout,
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

			VKUtils::SetImageLayout(transitionCmdBuffer, info.Image, VK_IMAGE_LAYOUT_UNDEFINED, m_Image->GetDescriptor().imageLayout, subResourceRange);
			device->FlushCommandBuffer(transitionCmdBuffer);
		}

		VkSamplerCreateInfo sampler {};
		sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler.maxAnisotropy = 1.0f;
		sampler.minFilter = VKUtils::GetSamplerFilter(m_Filter.MinFilter);
		sampler.magFilter = VKUtils::GetSamplerFilter(m_Filter.MagFilter);
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler.addressModeU = VKUtils::GetSamplerWrap(m_WrapMode);
		sampler.addressModeV = VKUtils::GetSamplerWrap(m_WrapMode);
		sampler.addressModeW = VKUtils::GetSamplerWrap(m_WrapMode);
		sampler.mipLodBias = 0.0f;
		sampler.compareOp = VK_COMPARE_OP_NEVER;
		sampler.minLod = 0.0f;
		sampler.maxLod = (float)1;

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
			view.subresourceRange.levelCount = 1;
			view.image = info.Image;
			VK_CHECK_RESULT(vkCreateImageView(device->GetDevice(), &view, nullptr, &info.ImageView));

			m_Image->UpdateDescriptor();
		}
	}
	bool VulkanTexture2D::LoadImageFromFile(const std::string& path)
	{
		TextureHeader header = TextureFactory::LoadFromCacheIfExists(path);
		m_Format = header.Channels == 4 ? ImageFormat::RGBA : ImageFormat::RGB;

		if (!header.IsValid())
			return false;

		m_Header = header;
		HZR_CORE_INFO("Loaded VulkanTexture2D from {0}", path);
		return true;
	}
}