#pragma once

#include <hzrpch.h>
#include "OpenGLTexture2D.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace Hazard::Rendering {

	OpenGLTexture2D::OpenGLTexture2D(const char* file)
	{
		int w, h, channels;
		stbi_uc* data = stbi_load(file, &w, &h, &channels, 0);

		if (!data) {
			HZR_CORE_WARN("Texture file not found");
			return;
		}

		width = w;
		height = h;

		internalFormat = (channels == 4) * GL_RGBA8 + (channels == 3) * GL_RGB8;
		dataFormat = (channels == 4) * GL_RGBA + (channels == 3) * GL_RGB;

		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
		glTextureStorage2D(textureID, 1, internalFormat, width, height);

		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glGenerateMipmap(textureID);
		glTexParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(const glm::vec2 size)
	{
		width = size.x;
		height = size.y;

		dataFormat = GL_RGBA;
		internalFormat = GL_RGBA8;

		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
		glTextureStorage2D(textureID, 1, GL_RGBA8, width, height);

		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &textureID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, textureID);
	}

	void OpenGLTexture2D::Unbind(uint32_t slot) const
	{
		glBindTextureUnit(slot, 0);
	}
	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = dataFormat == GL_RGBA ? 4 : 3;
		HZR_CORE_ASSERT(size == width * height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
	}
}