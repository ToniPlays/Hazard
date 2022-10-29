
#include "VulkanImage2D.h"
#ifdef HZR_INCLUDE_VULKAN
#include "Backend/Core/Renderer.h"
#include "Backend/Vulkan/VulkanContext.h"
#include "spdlog/fmt/fmt.h"

#include "../VkUtils.h"

namespace HazardRenderer::Vulkan
{
	VulkanImage2D::VulkanImage2D(Image2DCreateInfo* info)
	{
		HZR_ASSERT(!info->DebugName.empty(), "Debug name required");
		HZR_ASSERT(info->Format != ImageFormat::None, "Image format cannot be none");
		HZR_ASSERT(info->Usage != ImageUsage::None, "Image format cannot be none");

		m_DebugName = info->DebugName;
		m_Width = info->Width;
		m_Height = info->Height;
		m_Format = info->Format;
		m_Mips = info->GenerateMips ? VkUtils::GetMipLevelCount(m_Width, m_Height) : 1;
		m_Usage = info->Usage;

		m_ImageDescriptor.imageView = VK_NULL_HANDLE;
		m_ImageDescriptor.sampler = VK_NULL_HANDLE;

		Invalidate();

		if (!info->Data) return;

		m_LocalBuffer = Buffer::Copy(info->Data.Data, info->Data.Size);

		Ref<VulkanImage2D> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->UploadImageData_RT(instance->m_LocalBuffer, 
				instance->m_Mips > 1 ? VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL : instance->m_ImageDescriptor.imageLayout);

			if (instance->m_Mips > 1)
				instance->GenerateMips_RT();
			});

	}
	VulkanImage2D::~VulkanImage2D()
	{
		if (!m_Image) return;

		Ref<VulkanImage2D> instance = this;
		Renderer::SubmitResourceFree([instance]() mutable {
			instance->Release_RT();
			});

		m_LayerImageViews.clear();
	}
	void VulkanImage2D::Invalidate()
	{
		Ref<VulkanImage2D> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
			});
	}
	void VulkanImage2D::Release()
	{
		if (m_Image == nullptr) return;

		Ref<VulkanImage2D> instance = this;
		Renderer::SubmitResourceFree([instance]() mutable {
			instance->Release_RT();
			});

		m_Image = VK_NULL_HANDLE;
		m_ImageDescriptor.imageView = VK_NULL_HANDLE;
		m_ImageDescriptor.sampler = VK_NULL_HANDLE;
		m_LayerImageViews.clear();
		m_PerMipImageView.clear();
	}
	void VulkanImage2D::Release_RT()
	{
		if (m_Image == nullptr) return;

		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		vkDestroyImageView(device, m_ImageDescriptor.imageView, nullptr);
		vkDestroySampler(device, m_ImageDescriptor.sampler, nullptr);

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

		m_Image = VK_NULL_HANDLE;
		m_ImageDescriptor.imageView = VK_NULL_HANDLE;
		m_ImageDescriptor.sampler = VK_NULL_HANDLE;
		m_LayerImageViews.clear();
		m_PerMipImageView.clear();


	}
	void VulkanImage2D::Resize_RT(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
		Invalidate_RT();
	}
	void VulkanImage2D::Invalidate_RT()
	{
		HZR_ASSERT(m_Width > 0 && m_Height > 0, "Image dimensions failed");

		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		Release_RT();

		VulkanAllocator allocator("VulkanImage2D");

		VkImageUsageFlags flags = VK_IMAGE_USAGE_SAMPLED_BIT;
		if (m_Usage == ImageUsage::Attachment)
		{
			if (m_Format >= ImageFormat::DEPTH32F)
				flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			else
				flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}
		else if (m_Usage == ImageUsage::Texture)
		{
			flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}
		else if (m_Usage == ImageUsage::Storage)
		{
			flags |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		VkImageAspectFlags aspectMask = m_Format >= ImageFormat::DEPTH32F ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

		if (m_Format == ImageFormat::DEPTH24STENCIL8)
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;


		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.format = VkUtils::VulkanImageFormat(m_Format);
		createInfo.extent.width = m_Width;
		createInfo.extent.height = m_Height;
		createInfo.extent.depth = 1;
		createInfo.mipLevels = m_Mips;
		createInfo.arrayLayers = 1;
		createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		createInfo.usage = flags;

		m_ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		m_Allocation = allocator.AllocateImage(createInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Image);
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE, fmt::format("VulkanImage2D {0}", m_DebugName), m_Image);

		CreateImageView_RT();
		CreateSampler_RT();

		if (m_Usage == ImageUsage::Storage)
		{
			VkCommandBuffer commandBuffer = VulkanContext::GetLogicalDevice()->GetCommandBuffer(true);

			VkImageSubresourceRange range = {};
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.levelCount = m_Mips;
			range.layerCount = 1;

			VkUtils::InsertImageMemoryBarrier(commandBuffer, m_Image, 0, 0, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL,
				VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, range);

			VulkanContext::GetLogicalDevice()->FlushCommandBuffer(commandBuffer);
		}
	}
	void VulkanImage2D::UploadImageData_RT(Buffer data, VkImageLayout imageLayout)
	{
		auto& device = VulkanContext::GetLogicalDevice();

		VulkanAllocator allocator("VulkanImage2D");

		VkBufferCreateInfo stagingInfo = {};
		stagingInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingInfo.size = data.Size;
		stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stagingInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer stagingBuffer;
		VmaAllocation stagingBufferAlloc = allocator.AllocateBuffer(stagingInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

		//Copy image data
		uint8_t* destData = allocator.MapMemory<uint8_t>(stagingBufferAlloc);
		memcpy(destData, data.Data, data.Size);
		allocator.UnmapMemory(stagingBufferAlloc);
		data.Release();

		VkImageSubresourceRange range = {};
		range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		range.baseMipLevel = 0;
		range.levelCount = 1;
		range.layerCount = 1;

		VkCommandBuffer commandBuffer = device->GetCommandBuffer(true);

		VkUtils::InsertImageMemoryBarrier(commandBuffer, m_Image,
			VK_ACCESS_NONE, VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_ACCESS_HOST_READ_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, range);

		VkBufferImageCopy imageCopyRegion = {};
		imageCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.imageSubresource.mipLevel = 0;
		imageCopyRegion.imageSubresource.baseArrayLayer = 0;
		imageCopyRegion.imageSubresource.layerCount = 1;
		imageCopyRegion.imageExtent = { m_Width, m_Height };
		imageCopyRegion.imageExtent.depth = 1;
		imageCopyRegion.bufferOffset = 0;

		vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);

		VkUtils::InsertImageMemoryBarrier(commandBuffer, m_Image,
			VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageLayout,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, range);

		device->FlushCommandBuffer(commandBuffer);
		allocator.DestroyBuffer(stagingBuffer, stagingBufferAlloc);
	}
	void VulkanImage2D::CreateImageView_RT()
	{
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		if (m_ImageDescriptor.imageView)
			vkDestroyImageView(device, m_ImageDescriptor.imageView, nullptr);

		VkImageAspectFlags aspectMask = m_Format >= ImageFormat::DEPTH32F ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

		if (m_Format == ImageFormat::DEPTH24STENCIL8)
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.viewType = 1 > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D; //Change to support arrays
		viewInfo.format = VkUtils::VulkanImageFormat(m_Format);
		viewInfo.flags = 0;
		viewInfo.subresourceRange = {};
		viewInfo.subresourceRange.aspectMask = aspectMask;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = m_Mips;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.image = m_Image;

		VK_CHECK_RESULT(vkCreateImageView(device, &viewInfo, nullptr, &m_ImageDescriptor.imageView), "Failed to create VkImageView");
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("VkImageView {0}", m_DebugName), m_ImageDescriptor.imageView);
	}
	void VulkanImage2D::CreateSampler_RT()
	{
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		if (m_ImageDescriptor.sampler)
			vkDestroySampler(device, m_ImageDescriptor.sampler, nullptr);

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.maxAnisotropy = 1.0f;

		if (VkUtils::IsIntegratedBase(m_Format))
		{
			samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
			samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		}
		else
		{
			samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		}

		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU;
		samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeU;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.maxAnisotropy = 1.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = 100.0f;
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

		VK_CHECK_RESULT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &m_ImageDescriptor.sampler), "Failed to create VulkanImage2D sampler");
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SAMPLER, fmt::format("VkImageSampler {0}", m_DebugName), m_ImageDescriptor.sampler);
	}
	void VulkanImage2D::GenerateMips_RT()
	{
		const auto device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

		const VkCommandBuffer blitCmd = VulkanContext::GetInstance()->GetLogicalDevice()->GetCommandBuffer(true);

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = m_Image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		for (uint32_t mip = 1; mip < m_Mips; mip++)
		{
			VkImageBlit blit = {};
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.layerCount = 1;
			blit.srcSubresource.mipLevel = mip - 1;
			blit.srcOffsets[1].x = int32_t(m_Width >> (mip - 1));
			blit.srcOffsets[1].y = int32_t(m_Height >> (mip - 1));
			blit.srcOffsets[1].z = 1;

			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.layerCount = 1;
			blit.dstSubresource.mipLevel = mip;
			blit.dstOffsets[1].x = int32_t(m_Width >> mip);
			blit.dstOffsets[1].y = int32_t(m_Height >> mip);
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
		subRange.levelCount = m_Mips;

		VkUtils::InsertImageMemoryBarrier(blitCmd, m_Image,
			VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			subRange);


		VulkanContext::GetInstance()->GetLogicalDevice()->FlushCommandBuffer(blitCmd);

		CreateImageView_RT();
	}
}
#endif