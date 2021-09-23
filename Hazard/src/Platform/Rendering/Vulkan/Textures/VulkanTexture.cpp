#pragma once

#include <hzrpch.h>
#include "VulkanTexture.h"
#include "Platform/Rendering/Vulkan/Core/VulkanAllocator.h"
#include "Platform/Rendering/Vulkan/Buffers/VulkanImage.h"
#include "Platform/Rendering/Vulkan/VKContext.h"
#include "Platform/Rendering/Vulkan/Device/SwapChain.h"
#include "Platform/Rendering/Vulkan/Buffers/CommandBuffer.h"

#include <stb_image.h>


namespace Hazard::Rendering::Vulkan 
{
	VulkanTexture::VulkanTexture(const std::string& file)
	{
		auto device = VKContext::GetDevice();

		int channels;
		stbi_uc* data = stbi_load(file.c_str(), &m_Width, &m_Height, &channels, STBI_rgb_alpha);

		if (!data) {
			HZR_CORE_INFO("Failed to load image data");
			return;
		}

		VkDeviceSize imageSize = m_Width * m_Height * channels;
		
		VkBuffer stagingBuffer;

		VulkanAllocator allocator("StagingBuffer");

		VkBufferCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		info.size = imageSize;
		info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		
		auto allocBuffer = allocator.AllocateBuffer(&info, VMA_MEMORY_USAGE_CPU_TO_GPU, &stagingBuffer);
		void* dest = allocator.MapMemory<void>(allocBuffer);
		memcpy(dest, data, imageSize);

		stbi_image_free(data);

		VkExtent2D extent = { m_Width, m_Height };
		
		m_Image = VulkanImage::CreateImage(m_ImageMemory, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_IMAGE_TYPE_2D, extent, VK_FORMAT_R8G8B8A8_SRGB);

		allocator.TransitionImageLayuout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		allocator.CopyBufferToImage(stagingBuffer, m_Image, extent);
		allocator.TransitionImageLayuout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		m_Layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		allocator.UnmapMemory(allocBuffer);

		m_ImageView = VulkanImage::CreateImageView(m_Image, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
		m_ImageSampler = VulkanImage::CreateSampler();
	}
	VulkanTexture::~VulkanTexture()
	{
		auto device = VKContext::GetDevice()->GetDevice();
		vkDestroySampler(device, m_ImageSampler, nullptr);
		vkDestroyImageView(device, m_ImageView, nullptr);
		vkDestroyImage(device, m_Image, nullptr);
	}
	void VulkanTexture::Bind()
	{

	}
	void VulkanTexture::Bind(VkPipelineLayout layout)
	{

	}
	void VulkanTexture::Unbind()
	{

	}
}
