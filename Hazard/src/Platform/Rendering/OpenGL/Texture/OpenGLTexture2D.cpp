#pragma once

#include <hzrpch.h>
#include "OpenGLTexture2D.h"
#include "../OpenGLUtils.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace Hazard::Rendering::OpenGL {

	OpenGLTexture2D::OpenGLTexture2D(const Texture2DCreateInfo& info) : Texture2D(info)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		if (info.filename != "") {
			int w, h, channels;

			stbi_set_flip_vertically_on_load(1);

			if (info.datatype == TextureDataType::HDR)
			{
				float* data = stbi_loadf(info.filename.c_str(), &w, &h, &channels, 0);
				if (!data)
				{
					HZR_CORE_WARN("HDR Texture file not found {0}", info.filename);
					return;
				}

				m_Info.width = w;
				m_Info.height = h;

				glTexImage2D(GL_TEXTURE_2D, 0, OpenGLUtils::DataTypeToOpenGLType(info.datatype), w, h, 0, GL_RGB, GL_FLOAT, data);
				stbi_image_free(data);
			}
			else 
			{
				stbi_uc* data = stbi_load(info.filename.c_str(), &w, &h, &channels, 0);

				if (!data)
				{
					HZR_CORE_WARN("Texture file not found {0}", info.filename);
					return;
				}

				m_Info.width = w;
				m_Info.height = h;

				m_InternalFormat = (channels == 4) * GL_RGBA8 + (channels == 3) * GL_RGB8;
				m_DataFormat = (channels == 4) * GL_RGBA + (channels == 3) * GL_RGB;

				glTextureStorage2D(m_TextureID, 1, m_InternalFormat, m_Info.width, m_Info.height);
				glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Info.width, m_Info.height, m_DataFormat, GL_UNSIGNED_BYTE, data);
			}

			glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			return;
		}

		m_Info.width = info.width;
		m_Info.height = info.height;

		m_InternalFormat = info.datatype == TextureDataType::Auto ? GL_RGB8 : OpenGLUtils::DataTypeToOpenGLType(info.datatype);
		m_DataFormat = GL_RGBA;

		glTextureStorage2D(m_TextureID, 1, m_InternalFormat, info.width, info.height);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (info.data != nullptr) SetData(info.data, info.width * info.height);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}

	void OpenGLTexture2D::Unbind(uint32_t slot) const
	{
		glBindTextureUnit(slot, 0);
	}
	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		HZR_CORE_ASSERT(size == m_Info.width * m_Info.height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Info.width, m_Info.height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
}