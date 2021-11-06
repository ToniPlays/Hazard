
#pragma once

#include <hzrpch.h>
#include "OpenGLTexture2D.h"
#include "Hazard/File/File.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Hazard::Rendering::OpenGL
{
	namespace GLUtils {
		static GLenum TextureWrapToGLWrapMode(ImageWrap wrap) {
			switch (wrap)
			{
			case Repeat:		return GL_REPEAT;
			case RepeatMirror:	return GL_MIRRORED_REPEAT;
			case ClampBorder:	return GL_CLAMP_TO_BORDER;
			case ClampEdge:		return GL_CLAMP_TO_EDGE;
			}
		}
		static GLenum TextureFilterToGLFilter(FilterMode filter) {
			switch (filter)
			{
			case Linear:		return GL_LINEAR;
			case Nearest:		return GL_NEAREST;
			case LinearMip:		return GL_LINEAR_MIPMAP_LINEAR;
			case NearestMip:	return GL_NEAREST_MIPMAP_NEAREST;
			}
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(Texture2DCreateInfo* info)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		if (info->Data == nullptr)
			LoadFromFile(info->FilePath.string());
		else
			LoadFromData(info->Data, info->Width, info->Height);

		SetFilters(info->Filter, info->WrapMode);
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_ID);
	}
	void OpenGLTexture2D::LoadFromFile(const std::string& path)
	{
		int channels;
		std::string& file = File::GetFileAbsolutePath(path);

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(file.c_str(), &m_Width, &m_Height, &channels, 0);

		if (!data) {
			HZR_CORE_INFO("[OpenGLTexture2D]: Failed to load image data from {0}", file);
			return;
		}

		m_Format = (channels == 4) * GL_RGBA + (channels == 3) * GL_RGB;
		m_InternalFormat = (channels == 4) * GL_RGBA8 + (channels == 3) * GL_RGB8;

		glTextureStorage2D(m_ID, 1, m_InternalFormat, m_Width, m_Height);
		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
	}
	void OpenGLTexture2D::LoadFromData(const void* data, uint32_t width, uint32_t height)
	{

	}
	void OpenGLTexture2D::SetFilters(TextureFilter* filters, ImageWrap wrap)
	{
		if (filters == nullptr)
		{
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GLUtils::TextureFilterToGLFilter(filters->MinFilter));
			glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GLUtils::TextureFilterToGLFilter(filters->MagFilter));
		}

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GLUtils::TextureWrapToGLWrapMode(wrap));
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GLUtils::TextureWrapToGLWrapMode(wrap));
	}
}
