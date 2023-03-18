
#include "VulkanCubemapTexture.h"
#ifdef HZR_INCLUDE_VULKAN
#include "File.h"

#include "Backend/Core/Renderer.h"
#include "Backend/Core/Pipeline/Pipeline.h"
#include "../VulkanContext.h"
#include "../VkUtils.h"
#include "../VulkanRenderCommandBuffer.h"
#include "spdlog/fmt/fmt.h"
#include "VulkanImage2D.h"


#include "vendor/stb_image.h"

namespace HazardRenderer::Vulkan
{
	VulkanCubemapTexture::VulkanCubemapTexture(CubemapTextureCreateInfo* createInfo) : m_FilePath(createInfo->FilePath), m_Format(createInfo->Format)
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

		m_MipLevels = createInfo->GenerateMips ? VkUtils::GetMipLevelCount(m_Width, m_Height) : 1;

		Ref<VulkanCubemapTexture> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {

			const auto& device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

			VulkanAllocator allocator("VulkanCubemap");

			VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT;
			usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
			usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			usage |= VK_IMAGE_USAGE_STORAGE_BIT;

			VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;


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
			VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE, fmt::format("VulkanCubemap {0}", instance->m_DebugName), instance->m_Image);

			VkCommandBuffer layoutCmd = VulkanContext::GetLogicalDevice()->GetCommandBuffer(true);

			VkImageSubresourceRange subRange = {};
			subRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subRange.baseMipLevel = 0;
			subRange.levelCount = instance->m_MipLevels;
			subRange.layerCount = 6;

			VkUtils::SetImageLayout(layoutCmd, instance->m_Image, VK_IMAGE_LAYOUT_UNDEFINED, instance->m_ImageDescriptor.imageLayout, subRange);

			VulkanContext::GetLogicalDevice()->FlushCommandBuffer(layoutCmd);

			instance->CreateImageView_RT();
			instance->CreateSampler_RT();

			});

		if (createInfo->pCubemapSrc)
			GenerateFromCubemap(*createInfo->pCubemapSrc);

		if (createInfo->Data)
			SetImageData(createInfo->Data);
	}
	void VulkanCubemapTexture::Bind(uint32_t slot) const
	{
		HZR_PROFILE_FUNCTION();
		Renderer::Submit([s = slot]() mutable {
			HZR_PROFILE_FUNCTION("VulkanCubemapTexture::Bind(uint32_t) RT");
			});
	}
	void VulkanCubemapTexture::GenerateFromCubemap(CubemapGen& generationData)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(generationData.Pipeline, "No pipeline specified for cubemap generation");
		auto commandBuffer = VulkanContext::GetInstance()->GetSwapchain()->GetSwapchainBuffer();

		generationData.Pipeline->GetShader()->Set(generationData.OutputImageName, 0, this);

		DispatchComputeInfo computeInfo = {};
		computeInfo.GroupSize = { m_Width / 32, m_Height / 32, 6 };
		computeInfo.WaitForCompletion = true;

		commandBuffer->SetPipeline(generationData.Pipeline);
		commandBuffer->DispatchCompute(computeInfo);

		Ref<VulkanCubemapTexture> instance = this;
		Renderer::Submit([instance]() mutable {
			//instance->GenerateMipmaps_RT();
			});
	}
	void VulkanCubemapTexture::UploadImageData_RT()
	{
		auto device = VulkanContext::GetLogicalDevice();

		VulkanAllocator allocator("VulkanImage2D");

		VkBufferCreateInfo stagingInfo = {};
		stagingInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingInfo.size = m_LocalBuffer.Size;
		stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stagingInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer stagingBuffer;
		VmaAllocation stagingBufferAlloc = allocator.AllocateBuffer(stagingInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

		//Copy image data
		uint8_t* destData = allocator.MapMemory<uint8_t>(stagingBufferAlloc);
		memcpy(destData, m_LocalBuffer.Data, m_LocalBuffer.Size);
		allocator.UnmapMemory(stagingBufferAlloc);
		m_LocalBuffer.Release();

		VkImageSubresourceRange range = {};
		range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		range.baseMipLevel = 0;
		range.levelCount = 1;
		range.layerCount = 6;

		VkCommandBuffer commandBuffer = device->GetCommandBuffer(true);

		VkUtils::InsertImageMemoryBarrier(commandBuffer, m_Image,
			VK_ACCESS_NONE, VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_ACCESS_HOST_READ_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, range);

		VkBufferImageCopy imageCopyRegion = {};
		imageCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.imageSubresource.mipLevel = 0;
		imageCopyRegion.imageSubresource.baseArrayLayer = 0;
		imageCopyRegion.imageSubresource.layerCount = 6;
		imageCopyRegion.imageExtent = { m_Width, m_Height, 1 };
		imageCopyRegion.bufferOffset = 0;

		vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);

		VkUtils::InsertImageMemoryBarrier(commandBuffer, m_Image,
			VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, range);

		device->FlushCommandBuffer(commandBuffer);
		allocator.DestroyBuffer(stagingBuffer, stagingBufferAlloc);
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
	void VulkanCubemapTexture::CreateSampler_RT()
	{
		HZR_PROFILE_FUNCTION();
		const auto& device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

		if (m_ImageDescriptor.sampler)
			vkDestroySampler(device, m_ImageDescriptor.sampler, nullptr);

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

		if (VkUtils::IsIntegratedBase(m_Format))
		{
			samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
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

		VK_CHECK_RESULT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &m_ImageDescriptor.sampler), "Failed to create VulkanCubemap sampler");
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SAMPLER, fmt::format("VkImageSampler {0}", m_DebugName), m_ImageDescriptor.sampler);
	}
	void VulkanCubemapTexture::GenerateMipmaps_RT(VkCommandBuffer commandBuffer, VkImageLayout imageLayout)
	{
		HZR_PROFILE_FUNCTION();
		const auto device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

		for (uint32_t face = 0; face < 6; face++)
		{
			VkImageSubresourceRange range = {};
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.baseArrayLayer = face;
			range.levelCount = m_MipLevels;
			range.layerCount = 1;

			VkUtils::InsertImageMemoryBarrier(commandBuffer, m_Image,
				0, VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
				range);
		}
		for (uint32_t mip = 1; mip < m_MipLevels; mip++)
		{
			for (uint32_t face = 0; face < 6; face++)
			{
				VkImageBlit blit = {};
				// Source
				blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				blit.srcSubresource.layerCount = 1;
				blit.srcSubresource.mipLevel = mip - 1;
				blit.srcSubresource.baseArrayLayer = face;
				blit.srcOffsets[1].x = int32_t(m_Width >> (mip - 1));
				blit.srcOffsets[1].y = int32_t(m_Height >> (mip - 1));
				blit.srcOffsets[1].z = 1;

				// Destination
				blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				blit.dstSubresource.layerCount = 1;
				blit.dstSubresource.mipLevel = mip;
				blit.dstSubresource.baseArrayLayer = face;
				blit.dstOffsets[1].x = int32_t(m_Width >> mip);
				blit.dstOffsets[1].y = int32_t(m_Height >> mip);
				blit.dstOffsets[1].z = 1;

				VkImageSubresourceRange mipSubRange = {};
				mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				mipSubRange.baseMipLevel = mip;
				mipSubRange.baseArrayLayer = face;
				mipSubRange.levelCount = 1;
				mipSubRange.layerCount = 1;

				// Prepare current mip level as image blit destination
				VkUtils::InsertImageMemoryBarrier(commandBuffer, m_Image,
					0, VK_ACCESS_TRANSFER_WRITE_BIT,
					VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
					mipSubRange);

				// Blit from previous level
				vkCmdBlitImage(
					commandBuffer,
					m_Image,
					VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					m_Image,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					1,
					&blit,
					VK_FILTER_LINEAR);

				// Prepare current mip level as image blit source for next level
				VkUtils::InsertImageMemoryBarrier(commandBuffer, m_Image,
					VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
					mipSubRange);
			}
		}

		VkImageSubresourceRange subRange = {};
		subRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subRange.layerCount = 6;
		subRange.levelCount = m_MipLevels;

		m_ImageDescriptor.imageLayout = imageLayout;

		VkUtils::InsertImageMemoryBarrier(commandBuffer, m_Image,
			VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_ImageDescriptor.imageLayout,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			subRange);
	}
	void VulkanCubemapTexture::SetImageData(const Buffer& buffer)
	{
		m_LocalBuffer = Buffer::Copy(buffer);

		Ref<VulkanCubemapTexture> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->UploadImageData_RT();
			});
	}
}
#endif
