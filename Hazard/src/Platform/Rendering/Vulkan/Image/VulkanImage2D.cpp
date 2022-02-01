#pragma once

#include <hzrpch.h>
#include "VulkanImage2D.h"
#include "../VulkanContext.h"
#include "../VKUtils.h"
#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan 
{
	VulkanImage2D::VulkanImage2D(Image2DCreateInfo* info)
	{
		m_Width = info->Width;
		m_Height = info->Height;
		m_Mips = info->Mips;
		m_Info.Usage = info->Usage;
		m_Info.Format = info->Format;
	}
	VulkanImage2D::~VulkanImage2D()
	{
		m_Buffer.Release();

		if (!m_Info.Image) return;

		const auto device = VulkanContext::GetDevice()->GetDevice();
		vkDestroyImageView(device, m_Info.ImageView, nullptr);
		vkDestroySampler(device, m_Info.Sampler, nullptr);

		for (auto& view : m_PerLayerImageViews) {
			if (!view) continue;
			vkDestroyImageView(device, view, nullptr);
		}
		VulkanAllocator allocator("VulkanImage2D");
		allocator.DestroyImage(m_Info.Image, m_Info.MemoryAlloc);

		m_PerLayerImageViews.clear();
	}
	void VulkanImage2D::Invalidate()
	{
		RT_Invalidate();
	}
	void VulkanImage2D::RT_Invalidate()
	{
		Release();

		const auto device = VulkanContext::GetDevice()->GetDevice();
		VulkanAllocator allocator("Image2D");

		VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT;

		if (m_Info.Usage == ImageUsage::Attachment) {
			if (VKUtils::IsDepth(m_Info.Format))
				usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			else
				usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}
		else if (m_Info.Usage == ImageUsage::Texture) {
			usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}
		else if (m_Info.Usage == ImageUsage::Storage) {
			usage |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		VkImageAspectFlags aspectMask = VKUtils::IsDepth(m_Info.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

		if (m_Info.Format == ImageFormat::DEPTH24STENCIL8) 
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VkFormat vulkanFormat = VKUtils::GetFormat(m_Info.Format);

		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = vulkanFormat;
		imageCreateInfo.extent.width = m_Width;
		imageCreateInfo.extent.height = m_Height;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = m_Mips;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = usage;

		m_Info.MemoryAlloc = allocator.AllocateImage(imageCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Info.Image);

		// Create a default image view
		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = vulkanFormat;
		imageViewCreateInfo.flags = 0;
		imageViewCreateInfo.subresourceRange = {};
		imageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = m_Mips;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;
		imageViewCreateInfo.image = m_Info.Image;
		imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };


		VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_Info.ImageView));
		
		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.maxAnisotropy = 1.0f;
		samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU;
		samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeU;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.maxAnisotropy = 1.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = 100.0f;
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		VK_CHECK_RESULT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &m_Info.Sampler));
		

		if (m_Info.Usage == ImageUsage::Storage)
		{
			// Transition image to GENERAL layout
			VkCommandBuffer commandBuffer = VulkanContext::GetDevice()->GetCommandBuffer(true);

			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = m_Mips;
			subresourceRange.layerCount = 1;

			VKUtils::InsertImageMemoryBarrier(commandBuffer, m_Info.Image,
				0, 0,
				VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL,
				VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
				subresourceRange);

			VulkanContext::GetDevice()->FlushCommandBuffer(commandBuffer);
		}

		UpdateDescriptor();

	}
	void VulkanImage2D::Release()
	{
		if (m_Info.Image == nullptr) return;

		const auto device = VulkanContext::GetDevice()->GetDevice();
		vkDestroyImageView(device, m_Info.ImageView, nullptr);
		vkDestroySampler(device, m_Info.Sampler, nullptr);

		for (auto& view : m_PerLayerImageViews) {
			if (!view) continue;
			vkDestroyImageView(device, view, nullptr);
		}
		VulkanAllocator allocator("VulkanImage2D");
		allocator.DestroyImage(m_Info.Image, m_Info.MemoryAlloc);

		m_Info.Image = nullptr;
		m_Info.ImageView = nullptr;
		m_Info.Sampler = nullptr;

		m_PerLayerImageViews.clear();
		m_MipImageViews.clear();
	}
	void VulkanImage2D::Resize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
		RT_Invalidate();
	}
	void VulkanImage2D::UpdateDescriptor()
	{
		if (VKUtils::IsDepth(m_Info.Format)) 
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		else if (m_Info.Usage == ImageUsage::Storage) 
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		else 
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		if (m_Info.Usage == ImageUsage::Storage) {
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		}
		m_DescriptorImageInfo.imageView = m_Info.ImageView;
		m_DescriptorImageInfo.sampler = m_Info.Sampler;
	}
}