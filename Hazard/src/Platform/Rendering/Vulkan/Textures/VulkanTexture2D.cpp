
#pragma once

#include <hzrpch.h>
#include "VulkanTexture2D.h"
#include "Hazard/File/File.h"

#include <stb_image.h>


namespace Hazard::Rendering::Vulkan
{
	VulkanTexture2D::VulkanTexture2D(Texture2DCreateInfo* info)
	{
		int channels;

		std::string& file = File::GetFileAbsolutePath(info->filePath.string());

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(file.c_str(), &m_Width, &m_Height, &channels, STBI_rgb_alpha);

		if (!data) {
			HZR_CORE_INFO("[OpenGLTexture2D]: Failed to load image data from {0}", file);
			return;
		}
	}
	VulkanTexture2D::~VulkanTexture2D()
	{
	}
	void VulkanTexture2D::Bind(uint32_t slot) const
	{
	}
	void VulkanTexture2D::Unbind(uint32_t slot) const
	{
	}
}
