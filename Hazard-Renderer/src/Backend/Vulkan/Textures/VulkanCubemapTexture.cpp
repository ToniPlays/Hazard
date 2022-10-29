
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

		if (createInfo->pCubemapSrc)
		{
			m_Width = createInfo->pCubemapSrc->pCubemap->GetWidth();
			m_Height = createInfo->pCubemapSrc->pCubemap->GetWidth();
		}
		else
		{
			m_Width = createInfo->Width;
			m_Height = createInfo->Height;
		}

		m_ImageDescriptor.imageView = VK_NULL_HANDLE;
		m_ImageDescriptor.sampler = VK_NULL_HANDLE;

		m_MipLevels = createInfo->GenerateMips ? VkUtils::GetMipLevelCount(m_Width, m_Height) : 1;

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
		imageCreateInfo.format = VkUtils::VulkanImageFormat(m_Format);
		imageCreateInfo.extent.width = m_Width;
		imageCreateInfo.extent.height = m_Height;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = m_MipLevels;
		imageCreateInfo.arrayLayers = 6;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = usage;
		imageCreateInfo.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

		m_ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

		m_Allocation = allocator.AllocateImage(imageCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Image);
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE, fmt::format("VulkanCubemap {0}", m_DebugName), m_Image);

		VkCommandBuffer layoutCmd = VulkanContext::GetLogicalDevice()->GetCommandBuffer(true);

		VkImageSubresourceRange subRange = {};
		subRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subRange.baseMipLevel = 0;
		subRange.levelCount = m_MipLevels;
		subRange.layerCount = 6;

		VkUtils::SetImageLayout(layoutCmd, m_Image, VK_IMAGE_LAYOUT_UNDEFINED, m_ImageDescriptor.imageLayout, subRange);

		VulkanContext::GetLogicalDevice()->FlushCommandBuffer(layoutCmd);

		CreateImageView_RT();
		CreateSampler_RT();

		if (m_MipLevels > 1 && createInfo->Data)
			GenerateMipmaps_RT();

		if (createInfo->pCubemapSrc)
		{
			GenerateFromCubemap(*createInfo->pCubemapSrc);
		}
		else if (createInfo->pImageSrc)
		{
			Ref<Image2D> image = createInfo->pImageSrc->pSourceImage;
			GenerateFromEquirectangular(image);
		}
	}
	void VulkanCubemapTexture::Bind(uint32_t slot) const
	{
		HZR_PROFILE_FUNCTION();
		Renderer::Submit([s = slot]() mutable {
			HZR_PROFILE_FUNCTION("VulkanCubemapTexture::Bind(uint32_t) RT");
			});
	}
	void VulkanCubemapTexture::GenerateFromEquirectangular(Ref<Image2D> sourceImage)
	{
		HZR_PROFILE_FUNCTION();
		Ref<CubemapTexture> instance = this;

		PipelineSpecification pipelineSpec = {};
		pipelineSpec.DebugName = "EquirectangularToCubemap";
		pipelineSpec.ShaderPath = "res/Shaders/Compute/EquirectangularToCubeMap.glsl";
		pipelineSpec.Usage = PipelineUsage::ComputeBit;

		Ref<Pipeline> computePipeline = Pipeline::Create(&pipelineSpec);
		Renderer::Submit([inst = instance.As<VulkanCubemapTexture>()]() mutable {
			auto buffer = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>()->GetCurrentDrawCommandBuffer();
			VkImageSubresourceRange range = {};
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.levelCount = inst->m_MipLevels;
			range.baseArrayLayer = 0;
			range.layerCount = 6;

			VkUtils::InsertImageMemoryBarrier(
				buffer,
				inst->m_Image,
				VK_ACCESS_SHADER_READ_BIT, VK_ACCESS_TRANSFER_READ_BIT,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				VK_IMAGE_LAYOUT_GENERAL,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				range
			);
			inst->m_ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
			std::cout << "Transition of " << inst->m_DebugName << " to " << VkUtils::ImageLayoutToString(inst->m_ImageDescriptor.imageLayout) << std::endl;

		});

		auto& commandBuffer = VulkanContext::GetInstance()->GetSwapchain()->GetSwapchainBuffer();

		auto& shader = computePipeline->GetShader();
		shader->Set("o_CubeMap", 0, instance);
		shader->Set("u_EquirectangularTexture", 0, sourceImage);

		commandBuffer->BindPipeline(computePipeline);
		commandBuffer->DispatchCompute({ m_Width / 32, m_Height / 32, 6 });

		MemoryBarrierInfo barrier = {};
		barrier.Flags = MemoryBarrierBit_All;

		commandBuffer->InsertMemoryBarrier(barrier);

		Renderer::Submit([inst = instance.As<VulkanCubemapTexture>()]() mutable {
			auto buffer = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>()->GetCurrentDrawCommandBuffer();

			VkImageSubresourceRange range = {};
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.levelCount = inst->m_MipLevels;
			range.baseArrayLayer = 0;
			range.layerCount = 6;
			Timer timer;
			VkUtils::InsertImageMemoryBarrier(
				buffer,
				inst->m_Image,
				VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
				VK_IMAGE_LAYOUT_GENERAL,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				range
			);

			std::cout << timer.ElapsedMillis() << std::endl;
			inst->m_ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			if (inst->m_MipLevels > 1) 
{
				std::cout << "Transition of " << inst->m_DebugName << " to " << VkUtils::ImageLayoutToString(inst->m_ImageDescriptor.imageLayout) << std::endl;
				inst->GenerateMipmaps_RT(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			}
		});
	}
	void VulkanCubemapTexture::GenerateFromCubemap(CubemapGen& generationData)
	{
		HZR_PROFILE_FUNCTION();
		Timer timer;
		HZR_ASSERT(generationData.Pipeline, "No pipeline specified for cubemap generation");
		auto& commandBuffer = VulkanContext::GetInstance()->GetSwapchain()->GetSwapchainBuffer();

		Renderer::Submit([inst = generationData.pCubemap.As<VulkanCubemapTexture>()]() mutable {
			auto buffer = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>()->GetCurrentDrawCommandBuffer();

			VkImageSubresourceRange range = {};
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.levelCount = inst->m_MipLevels;
			range.baseArrayLayer = 0;
			range.layerCount = 6;
			Timer timer;
			VkUtils::InsertImageMemoryBarrier(
				buffer,
				inst->m_Image,
				VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				VK_IMAGE_LAYOUT_GENERAL,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				range
			);
		});

		Ref<CubemapTexture> instance = this;
		generationData.Pipeline->GetShader()->Set(generationData.OutputImageName, 0, instance);

		commandBuffer->BindPipeline(generationData.Pipeline);
		commandBuffer->DispatchCompute({ m_Width / 32, m_Height / 32, 6 });

		MemoryBarrierInfo barrier = {};
		barrier.Flags = MemoryBarrierBit_All;

		commandBuffer->InsertMemoryBarrier(barrier);

		Renderer::Submit([inst = instance.As<VulkanCubemapTexture>()]() mutable {
			auto buffer = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>()->GetCurrentDrawCommandBuffer();

			VkImageSubresourceRange range = {};
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.levelCount = inst->m_MipLevels;
			range.baseArrayLayer = 0;
			range.layerCount = 6;
			Timer timer;
			VkUtils::InsertImageMemoryBarrier(
				buffer,
				inst->m_Image,
				VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
				VK_IMAGE_LAYOUT_GENERAL,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				range
			);

			std::cout << timer.ElapsedMillis() << std::endl;
			inst->m_ImageDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			if (inst->m_MipLevels > 1)
			{
				std::cout << "Transition of " << inst->m_DebugName << " to " << VkUtils::ImageLayoutToString(inst->m_ImageDescriptor.imageLayout) << std::endl;
				inst->GenerateMipmaps_RT(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			}
		});

		Renderer::Submit([inst = generationData.pCubemap.As<VulkanCubemapTexture>()]() mutable {
			auto buffer = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>()->GetCurrentDrawCommandBuffer();

			VkImageSubresourceRange range = {};
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.levelCount = inst->m_MipLevels;
			range.baseArrayLayer = 0;
			range.layerCount = 6;
			Timer timer;
			VkUtils::InsertImageMemoryBarrier(
				buffer,
				inst->m_Image,
				VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
				VK_IMAGE_LAYOUT_GENERAL,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				range
			);
		});

	}
	void VulkanCubemapTexture::CreateImageView_RT()
	{
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
		const auto& device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

		if (m_ImageDescriptor.sampler)
			vkDestroySampler(device, m_ImageDescriptor.sampler, nullptr);

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.maxAnisotropy = 1.0f;

		if (VkUtils::IsIntegratedBase(m_Format))
		{
			samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
			samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
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
	void VulkanCubemapTexture::GenerateMipmaps_RT(VkImageLayout imageLayout)
	{
		const auto device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

		const VkCommandBuffer blitCmd = VulkanContext::GetInstance()->GetLogicalDevice()->GetCommandBuffer(true);

		for (uint32_t face = 0; face < 6; face++)
		{
			VkImageSubresourceRange range = {};
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.baseArrayLayer = face;
			range.levelCount = m_MipLevels;
			range.layerCount = 1;

			VkUtils::InsertImageMemoryBarrier(blitCmd, m_Image,
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
				VkUtils::InsertImageMemoryBarrier(blitCmd, m_Image,
					0, VK_ACCESS_TRANSFER_WRITE_BIT,
					VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
					mipSubRange);

				// Blit from previous level
				vkCmdBlitImage(
					blitCmd,
					m_Image,
					VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					m_Image,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					1,
					&blit,
					VK_FILTER_LINEAR);

				// Prepare current mip level as image blit source for next level
				VkUtils::InsertImageMemoryBarrier(blitCmd, m_Image,
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

		VkUtils::InsertImageMemoryBarrier(blitCmd, m_Image,
			VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_ImageDescriptor.imageLayout,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			subRange);

		std::cout << "Mipmap Transition of " << m_DebugName << " to " << VkUtils::ImageLayoutToString(m_ImageDescriptor.imageLayout) << std::endl;
		VulkanContext::GetInstance()->GetLogicalDevice()->FlushCommandBuffer(blitCmd);
	}
}
#endif