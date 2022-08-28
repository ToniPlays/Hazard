#include "VulkanImage2D.h"
#include "Backend/Core/Renderer.h"
#include "Backend/Vulkan/VulkanContext.h"

#include "../VkUtils.h"

namespace HazardRenderer::Vulkan
{
	VulkanImage2D::VulkanImage2D(Image2DCreateInfo* info)
	{
		HZR_ASSERT(info->Format != ImageFormat::None, "Image format cannot be none");
		HZR_ASSERT(info->Usage != ImageUsage::None, "Image format cannot be none");

		m_Width = info->Width;
		m_Height = info->Height;
		m_Format = info->Format;
		m_Mips = info->Mips;
		m_Usage = info->Usage;

		
		Invalidate();

		if (info->Data) 
		{
			m_LocalBuffer = Buffer::Copy(info->Data.Data, info->Data.Size);
			info->Data.Release();

			Ref<VulkanImage2D> instance = this;
			Renderer::SubmitResourceCreate([instance]() mutable {
				instance->UploadImageData_RT(instance->m_LocalBuffer);
				});
		}

	}
	VulkanImage2D::~VulkanImage2D()
	{
		if (!m_Info.Image) return;

		const VulkanImageInfo& info = m_Info;
		Renderer::SubmitResourceFree([info, layerViews = m_LayerImageViews]() mutable {
			const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

			vkDestroyImageView(device, info.ImageView, nullptr);
			vkDestroySampler(device, info.Sampler, nullptr);

			for (auto& view : layerViews)
			{
				if (view) vkDestroyImageView(device, view, nullptr);
			}

			VulkanAllocator allocator("VulkanImage2D");
			allocator.DestroyImage(info.Image, info.Allocation);
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
		if (m_Info.Image == nullptr) return;

		Ref<VulkanImage2D> instance = this;
		const VulkanImageInfo info = m_Info;
		Renderer::SubmitResourceFree([instance, info, mipViews = m_PerMipImageView, layerViews = m_LayerImageViews]() mutable {
			const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
			vkDestroyImageView(device, info.ImageView, nullptr);
			vkDestroySampler(device, info.Sampler, nullptr);

			for (auto& [index, view] : mipViews)
			{
				if (view) vkDestroyImageView(device, view, nullptr);
			}
			for (auto& view : layerViews)
				if (view) vkDestroyImageView(device, view, nullptr);

			VulkanAllocator allocator("VulkanImage2D");
			allocator.DestroyImage(info.Image, info.Allocation);
			});

		m_Info.Image = VK_NULL_HANDLE;
		m_Info.ImageView = VK_NULL_HANDLE;
		m_Info.Sampler = VK_NULL_HANDLE;
		m_LayerImageViews.clear();
		m_PerMipImageView.clear();
	}
	void VulkanImage2D::Release_RT()
	{
		if (m_Info.Image == nullptr) return;

		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
		vkDestroyImageView(device, m_Info.ImageView, nullptr);
		vkDestroySampler(device, m_Info.Sampler, nullptr);

		for (auto& [index, view] : m_PerMipImageView)
		{
			if (view) vkDestroyImageView(device, view, nullptr);
		}
		for (auto& view : m_LayerImageViews)
			if (view) vkDestroyImageView(device, view, nullptr);

		VulkanAllocator allocator("VulkanImage2D");
		allocator.DestroyImage(m_Info.Image, m_Info.Allocation);

		m_Info.Image = VK_NULL_HANDLE;
		m_Info.ImageView = VK_NULL_HANDLE;
		m_Info.Sampler = VK_NULL_HANDLE;
		m_LayerImageViews.clear();
		m_PerMipImageView.clear();
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

		VkFormat vulkanFormat = VkUtils::VulkanImageFormat(m_Format);

		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.format = vulkanFormat;
		createInfo.extent.width = m_Width;
		createInfo.extent.height = m_Height;
		createInfo.extent.depth = 1;
		createInfo.mipLevels = m_Mips;
		createInfo.arrayLayers = 1;
		createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		createInfo.usage = flags;

		m_Info.Allocation = allocator.AllocateImage(createInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Info.Image);
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE, "VulkanImage2D", m_Info.Image);

		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.viewType = 1 > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D; //Change to support arrays
		viewInfo.format = vulkanFormat;
		viewInfo.flags = 0;
		viewInfo.subresourceRange = {};
		viewInfo.subresourceRange.aspectMask = aspectMask;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = m_Mips;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		viewInfo.image = m_Info.Image;
		VK_CHECK_RESULT(vkCreateImageView(device, &viewInfo, nullptr, &m_Info.ImageView), "Failed to create VkImageView");
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, "VulkanImage2D ImageView", m_Info.ImageView);

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

		VK_CHECK_RESULT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &m_Info.Sampler), "Failed to create VulkanImage2D sampler");
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SAMPLER, "VulkanImage2D sampler", m_Info.Sampler);

		if (m_Usage == ImageUsage::Storage)
		{
			VkCommandBuffer commandBuffer = VulkanContext::GetLogicalDevice()->GetCommandBuffer(true);

			VkImageSubresourceRange range = {};
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.levelCount = m_Mips;
			range.layerCount = 1;

			VkUtils::InsertImageMemoryBarrier(commandBuffer, m_Info.Image, 0, 0, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL,
				VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, range);

			VulkanContext::GetLogicalDevice()->FlushCommandBuffer(commandBuffer);
		}

		UpdateDescriptor();
	}
	void VulkanImage2D::UpdateDescriptor()
	{
		if (m_Format == ImageFormat::DEPTH24STENCIL8 || m_Format == ImageFormat::DEPTH32F)
			m_ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		else if (m_Usage == ImageUsage::Storage)
			m_ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		else
			m_ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		m_ImageDescriptor.imageView = m_Info.ImageView;
		m_ImageDescriptor.sampler = m_Info.Sampler;
	}
	void VulkanImage2D::UploadImageData_RT(Buffer data)
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

		VkUtils::InsertImageMemoryBarrier(commandBuffer, m_Info.Image, 
			0, VK_ACCESS_TRANSFER_WRITE_BIT, 
			VK_IMAGE_LAYOUT_UNDEFINED,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_ACCESS_HOST_READ_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, range);

		VkBufferImageCopy imageCopyRegion = {};
		imageCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.imageSubresource.mipLevel = 0;
		imageCopyRegion.imageSubresource.baseArrayLayer = 0;
		imageCopyRegion.imageSubresource.layerCount = 1;
		imageCopyRegion.imageExtent = { m_Width, m_Height };
		imageCopyRegion.imageExtent.depth = 1;
		imageCopyRegion.bufferOffset = 0;

		vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, m_Info.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);

		VkUtils::InsertImageMemoryBarrier(commandBuffer, m_Info.Image,
			VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_ImageDescriptor.imageLayout,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, range);

		device->FlushCommandBuffer(commandBuffer);

		allocator.DestroyBuffer(stagingBuffer, stagingBufferAlloc);
	}
}
