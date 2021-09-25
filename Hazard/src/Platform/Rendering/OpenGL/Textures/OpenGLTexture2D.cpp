
#pragma once

#include <hzrpch.h>
#include "OpenGLTexture2D.h"
#include "Hazard/File/File.h"

#include <stb_image.h>


namespace Hazard::Rendering::OpenGL
{
	OpenGLTexture2D::OpenGLTexture2D(Texture2DCreateInfo* info)
	{
		int channels;

		std::string& file = File::GetFileAbsolutePath(info->filePath.string());

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(file.c_str(), &m_Width, &m_Height, &channels, STBI_rgb_alpha);

		if (!data) {
			HZR_CORE_INFO("[OpenGLTexture2D]: Failed to load image data from {0}", file);
			return;
		}

		m_Format = (channels == 4) * GL_RGBA + (channels == 3) * GL_RGB;
		m_InternalFormat = (channels == 4) * GL_RGBA8 + (channels == 3) * GL_RGB8;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		
		glTextureStorage2D(m_ID, 1, m_InternalFormat, m_Width, m_Height);
		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
	}
	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_ID);
	}
	void OpenGLTexture2D::Unbind(uint32_t slot) const
	{
		glBindTextureUnit(slot, 0);
	}
}
