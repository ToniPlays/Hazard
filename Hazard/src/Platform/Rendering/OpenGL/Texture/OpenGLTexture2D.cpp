#pragma once

#include <hzrpch.h>
#include "OpenGLTexture2D.h"
#include "../OpenGLUtils.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace Hazard::Rendering::OpenGL {

	OpenGLTexture2D::OpenGLTexture2D(const char* file, TextureSpecs params) : Texture2D(file)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		if (params.diskImage) {
			int w, h, channels;

			stbi_set_flip_vertically_on_load(1);

			if (params.dataType == TextureDataType::HDR) 
			{
				float* data = stbi_loadf(file, &w, &h, &channels, 0);
				if (!data)
				{
					HZR_CORE_WARN("Texture file not found");
					return;
				}

				m_Spec.width = w;
				m_Spec.height = h;

				glTexImage2D(GL_TEXTURE_2D, 0, OpenGLUtils::DataTypeToOpenGLType(params.dataType), w, h, 0, GL_RGB, GL_FLOAT, data);
				stbi_image_free(data);
			}
			else 
			{
				stbi_uc* data = stbi_load(file, &w, &h, &channels, 0);
				if (!data)
				{
					HZR_CORE_WARN("Texture file not found");
					return;
				}

				m_Spec.width = w;
				m_Spec.height = h;

				m_InternalFormat = (channels == 4) * GL_RGBA8 + (channels == 3) * GL_RGB8;
				m_DataFormat = (channels == 4) * GL_RGBA + (channels == 3) * GL_RGB;

				glTextureStorage2D(m_TextureID, 1, m_InternalFormat, m_Spec.width, m_Spec.height);
				glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Spec.width, m_Spec.height, m_DataFormat, GL_UNSIGNED_BYTE, data);
			}

			HZR_CORE_INFO("Loading texture from file {0}", file);

			glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			return;
		}

		HZR_CORE_INFO("Creating texture from nothing");

		m_Spec.width = params.width;
		m_Spec.height = params.height;

		m_InternalFormat = m_Spec.dataType == TextureDataType::Auto ? GL_RGB8 : OpenGLUtils::DataTypeToOpenGLType(m_Spec.dataType);
		m_DataFormat = GL_RGBA;

		glTextureStorage2D(m_TextureID, 1, m_InternalFormat, m_Spec.width, m_Spec.height);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
		HZR_CORE_ASSERT(size == m_Spec.width * m_Spec.height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Spec.width, m_Spec.height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
}