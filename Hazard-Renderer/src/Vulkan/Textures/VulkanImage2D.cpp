
#include "VulkanImage2D.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Core/Renderer.h"
#include "Vulkan/VulkanContext.h"
#include "spdlog/fmt/fmt.h"

#include "../VkUtils.h"
#include <Vulkan/VulkanRenderCommandBuffer.h>

namespace HazardRenderer::Vulkan
{
	VulkanImage2D::VulkanImage2D(Image2DCreateInfo* info)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(!info->DebugName.empty(), "Debug name required");
		HZR_ASSERT(info->Format != ImageFormat::None, "Image format cannot be none");
		HZR_ASSERT(info->Usage != ImageUsage::None, "Image usage cannot be none");

		HZR_ASSERT(info->Extent.Width < 32768 && info->Extent.Height < 32768, "Image extent too large");

		m_Info = *info;
		m_Info.Mips = 1;

		m_ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		m_ImageDescriptor.imageView = VK_NULL_HANDLE;
		m_ImageDescriptor.sampler = VK_NULL_HANDLE;

		Invalidate();

		if (!info->Data.Data) return;

		ImageCopyRegion region = {};
		region.Extent = m_Info.Extent;
		region.X = 0;
		region.Y = 0;
		region.Z = 0;
		region.Data = info->Data.Data;
		region.DataSize = info->Data.Size;

		Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::Create("ImageUpload", DeviceQueue::TransferBit, 1);
		cmdBuffer->Begin();
		cmdBuffer->CopyToImage(this, region);
		cmdBuffer->End();
		cmdBuffer->Submit();
	}

	VulkanImage2D::~VulkanImage2D()
	{
		HZR_PROFILE_FUNCTION();
		if (!m_Image) return;

		Renderer::SubmitResourceFree([descriptor = m_ImageDescriptor, mips = m_PerMipImageView, layers = m_LayerImageViews, image = m_Image, alloc = m_Allocation]() mutable {
			const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

			vkDestroyImageView(device, descriptor.imageView, nullptr);
			vkDestroySampler(device, descriptor.sampler, nullptr);

			for (auto& [index, view] : mips)
			{
				if (!view) continue;
				vkDestroyImageView(device, view, nullptr);
			}
			for (auto& view : layers)
			{
				if (!view) continue;
				vkDestroyImageView(device, view, nullptr);
			}

			VulkanAllocator allocator("VulkanImage2D");
			allocator.DestroyImage(image, alloc);

		});

		m_LayerImageViews.clear();
		m_PerMipImageView.clear();
	}

	void VulkanImage2D::Invalidate()
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanImage2D> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
		});
	}

	void VulkanImage2D::Release()
	{
		HZR_PROFILE_FUNCTION();
		if (m_Image == nullptr) return;

		Ref<VulkanImage2D> instance = this;
		Renderer::SubmitResourceFree([instance]() mutable {
			instance->Release_RT();
		});

		m_LayerImageViews.clear();
		m_PerMipImageView.clear();
	}

	void VulkanImage2D::Release_RT()
	{
		HZR_PROFILE_FUNCTION();
		if (m_Image == nullptr) return;

		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		vkDestroyImageView(device, m_ImageDescriptor.imageView, nullptr);
		m_ImageDescriptor.imageView = VK_NULL_HANDLE;

		for (auto& [index, view] : m_PerMipImageView)
		{
			if (!view) continue;
			vkDestroyImageView(device, view, nullptr);
		}
		for (auto& view : m_LayerImageViews)
		{
			if (!view) continue;
			vkDestroyImageView(device, view, nullptr);
		}

		VulkanAllocator allocator("VulkanImage2D");
		allocator.DestroyImage(m_Image, m_Allocation);

		m_LayerImageViews.clear();
		m_PerMipImageView.clear();
	}

	void VulkanImage2D::Resize_RT(uint32_t width, uint32_t height)
	{
		HZR_PROFILE_FUNCTION();
		m_Info.Extent.Width = width;
		m_Info.Extent.Height = height;
		Invalidate_RT();
	}

	Buffer VulkanImage2D::ReadPixels(const ImageCopyRegion& region)
	{
		VulkanAllocator allocator("VulkanImage2D");

		VkBufferCreateInfo stagingInfo = {};
		stagingInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingInfo.size = region.Extent.Width * region.Extent.Height * region.Extent.Depth * sizeof(uint64_t);
		stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		stagingInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer stagingBuffer;
		VmaAllocation stagingBufferAlloc = allocator.AllocateBuffer(stagingInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

		VkImageSubresourceLayers resource = {};
		resource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		resource.layerCount = 1;
		resource.baseArrayLayer = 0;
		resource.mipLevel = 0;

		VkBufferImageCopy copyRegion = {};
		copyRegion.imageExtent = { region.Extent.Width, region.Extent.Height, region.Extent.Depth };
		copyRegion.imageOffset = { (int)region.X, (int)region.Y, (int)region.Z };
		copyRegion.imageSubresource = resource;

		VkImageSubresourceRange range = {};
		range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		range.layerCount = 1;
		range.levelCount = 1;
		range.baseMipLevel = 0;

		VkCommandBuffer cmdBuffer = VulkanContext::GetLogicalDevice()->GetCommandBuffer(true);

		VkUtils::SetImageLayout(cmdBuffer, m_Image, m_ImageDescriptor.imageLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, range, VK_SHADER_STAGE_FRAGMENT_BIT, VK_SHADER_STAGE_FRAGMENT_BIT);
		vkCmdCopyImageToBuffer(cmdBuffer, m_Image, m_ImageDescriptor.imageLayout, stagingBuffer, 1, &copyRegion);	//TODO: Fix me
		VkUtils::SetImageLayout(cmdBuffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_ImageDescriptor.imageLayout, range, VK_SHADER_STAGE_FRAGMENT_BIT, VK_SHADER_STAGE_FRAGMENT_BIT);

		VulkanContext::GetLogicalDevice()->FlushCommandBuffer(cmdBuffer);

		uint8_t* imageData = allocator.MapMemory<uint8_t>(stagingBufferAlloc);

		Buffer buffer;
		buffer.Allocate(stagingInfo.size);
		memcpy(buffer.Data, imageData, buffer.Size);

		allocator.UnmapMemory(stagingBufferAlloc);
		allocator.DestroyBuffer(stagingBuffer, stagingBufferAlloc);
		return buffer;
	}

	void VulkanImage2D::Invalidate_RT()
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(m_Info.Extent.Width > 0 && m_Info.Extent.Height > 0, "Image dimensions failed");

		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		Release_RT();

		VulkanAllocator allocator("VulkanImage2D");

		VkImageUsageFlags flags = VK_IMAGE_USAGE_SAMPLED_BIT;
		if (m_Info.Usage == ImageUsage::Attachment)
			flags |= m_Info.Format >= ImageFormat::DEPTH32F ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		else if (m_Info.Usage == ImageUsage::Texture)
			flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		else if (m_Info.Usage == ImageUsage::Storage)
			flags |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		VkImageAspectFlags aspectMask = m_Info.Format >= ImageFormat::DEPTH32F ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

		if (m_Info.Format == ImageFormat::DEPTH24STENCIL8)
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.format = VkUtils::VulkanImageFormat(m_Info.Format);
		createInfo.extent.width = m_Info.Extent.Width;
		createInfo.extent.height = m_Info.Extent.Height;
		createInfo.extent.depth = 1;
		createInfo.mipLevels = m_Info.Mips;
		createInfo.arrayLayers = 1;
		createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		createInfo.usage = flags;

		m_ImageDescriptor.imageLayout = m_Info.Usage == ImageUsage::Storage ? VK_IMAGE_LAYOUT_GENERAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		m_Allocation = allocator.AllocateImage(createInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Image);
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE, fmt::format("VkImage2D {0}", m_Info.DebugName), m_Image);

		CreateImageView_RT();
	}

	void VulkanImage2D::CreateImageView_RT()
	{
		HZR_PROFILE_FUNCTION();
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		if (m_ImageDescriptor.imageView)
			vkDestroyImageView(device, m_ImageDescriptor.imageView, nullptr);
		m_ImageDescriptor.imageView = VK_NULL_HANDLE;

		VkImageAspectFlags aspectMask = m_Info.Format >= ImageFormat::DEPTH32F ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

		if (m_Info.Format == ImageFormat::DEPTH24STENCIL8)
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.viewType = 1 > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D; //TODO: Change to support arrays
		viewInfo.format = VkUtils::VulkanImageFormat(m_Info.Format);
		viewInfo.flags = 0;
		viewInfo.subresourceRange = {};
		viewInfo.subresourceRange.aspectMask = aspectMask;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = m_Info.Mips;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.image = m_Image;

		VK_CHECK_RESULT(vkCreateImageView(device, &viewInfo, nullptr, &m_ImageDescriptor.imageView), "Failed to create VkImageView");
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("VkImageView {0}", m_Info.DebugName), m_ImageDescriptor.imageView);
	}

	void VulkanImage2D::GenerateMips_RT()
	{
		/*
		HZR_PROFILE_FUNCTION();
		const auto device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

		const VkCommandBuffer blitCmd = VulkanContext::GetInstance()->GetLogicalDevice()->GetCommandBuffer(true);

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = m_Image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		for (uint32_t mip = 1; mip < m_Info.Mips; mip++)
		{
			VkImageBlit blit = {};
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.layerCount = 1;
			blit.srcSubresource.mipLevel = mip - 1;
			blit.srcOffsets[1].x = int32_t(m_Info.Extent.Width >> (mip - 1));
			blit.srcOffsets[1].y = int32_t(m_Info.Extent.Height >> (mip - 1));
			blit.srcOffsets[1].z = 1;

			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.layerCount = 1;
			blit.dstSubresource.mipLevel = mip;
			blit.dstOffsets[1].x = int32_t(m_Info.Extent.Width >> mip);
			blit.dstOffsets[1].y = int32_t(m_Info.Extent.Height >> mip);
			blit.dstOffsets[1].z = 1;

			VkImageSubresourceRange mipRange = {};
			mipRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			mipRange.baseMipLevel = mip;
			mipRange.levelCount = 1;
			mipRange.layerCount = 1;

			VkUtils::InsertImageMemoryBarrier(blitCmd, m_Image,
											  0, VK_ACCESS_TRANSFER_WRITE_BIT,
											  VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
											  VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
											  mipRange);

			vkCmdBlitImage(blitCmd, m_Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						   m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

			VkUtils::InsertImageMemoryBarrier(blitCmd, m_Image,
											  VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
											  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
											  VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
											  mipRange);

		}

		VkImageSubresourceRange subRange = {};
		subRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subRange.layerCount = 1;
		subRange.levelCount = m_Info.Mips;

		VkUtils::InsertImageMemoryBarrier(blitCmd, m_Image,
										  VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
										  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										  VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
										  subRange);


		VulkanContext::GetInstance()->GetLogicalDevice()->FlushCommandBuffer(blitCmd);
		CreateImageView_RT();

		*/
	}
}
#endif